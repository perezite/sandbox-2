#include "Board.h"

namespace blocks {
	void Board::createTetromino(Tetromino& tetromino, const sb::Vector2i& boardPosition) {
		tetromino.setPosition(boardToWorldPosition(boardPosition));
		tetromino.setScale(getCellSize());
		_tetromino = tetromino;
		_tetromino.setLight(_light);
		_hasTetromino = true;
	}

	void Board::createRandomTetromino() {
		const std::vector<char> types = Tetromino::getTypes();
		createTetromino(types[rand() % types.size()]);
		if (isInvalid(_tetromino))
			die();
	}

	sb::Vector2f Board::boardToWorldPosition(const sb::Vector2i& boardPos) {
		sb::Vector2f size(1, _boardSize.y / float(_boardSize.x));
		sb::Vector2f halfSize = 0.5f * size;
		sb::Vector2f delta(size.x / _boardSize.x, size.y / _boardSize.y);
		return sb::Vector2f(-halfSize.x + (boardPos.x + 0.5f) * delta.x,
			-halfSize.y + (boardPos.y + 0.5f) * delta.y);
	}


	sb::Vector2i Board::worldToBoardPosition(const sb::Vector2f& worldPos) {
		sb::Vector2f size(1, _boardSize.y / float(_boardSize.x));
		sb::Vector2f delta(size.x / _boardSize.x, size.y / _boardSize.y);
		sb::Vector2f adjustedPos = worldPos + 0.5f * size;
		auto result = sb::Vector2i(floori(adjustedPos.x / delta.x), floori(adjustedPos.y / delta.y));
		return result;
	}

	sb::Vector2f Board::getCellSize() {
		return sb::Vector2f(1.f / _boardSize.x, 1.f / _boardSize.x);
	}

	void Board::moveBy(Tetromino& tetromino, const sb::Vector2i& translation) {
		sb::Vector2i position = worldToBoardPosition(tetromino.getPosition());
		position += translation;
		tetromino.setPosition(boardToWorldPosition(position));
	}

	void Board::driftBy(Tetromino& tetromino, const sb::Vector2i& translation, float effectSeconds) {
		sb::Vector2i position = worldToBoardPosition(tetromino.getPosition());
		position += translation;
		tetromino.getEffects().driftTo(boardToWorldPosition(position), tetromino, effectSeconds);
	}

	bool Board::isOccupied(const sb::Vector2i& position) {
		for (size_t i = 0; i < _blocks.size(); i++) {
			const sb::Vector2i blockPosition = worldToBoardPosition(_blocks[i].getPosition());
			bool isBlockAlive = _blocks[i].getState() == Block::State::Alive;
			if (blockPosition == position && isBlockAlive)
				return true;
		}

		return false;
	}

	bool Board::isOutsideBoard(const sb::Vector2i& position) {
		return position.x < 0 || position.x >= _boardSize.x ||
			position.y < 0 || position.y >= _boardSize.y;
	}

	bool Board::isInvalid(Tetromino& tetromino) {
		const std::vector<sb::Vector2f> blockPositions = tetromino.getBlockPositions();

		for (std::size_t i = 0; i < blockPositions.size(); i++) {
			sb::Vector2i boardPosition = worldToBoardPosition(blockPositions[i]);
			if (isOccupied(boardPosition) || isOutsideBoard(boardPosition))
				return true;
		}

		return false;
	}

	void Board::freeze(Tetromino& tetromino) {
		char type = tetromino.getType();
		const std::vector<sb::Vector2f> blockPositions = tetromino.getBlockPositions();
		_hasTetromino = false;
		tetromino.die();
		_dyingTetrominoes.push_back(_tetromino);

		for (size_t i = 0; i < blockPositions.size(); i++) {
			sb::Vector2i boardPosition = worldToBoardPosition(blockPositions[i]);
			createBlock(type, boardPosition);
		}
	}

	bool Board::isLineFull(size_t y) {
		for (int x = 0; x < _boardSize.x; x++) {
			if (!isOccupied(sb::Vector2i(x, y)))
				return false;
		}

		return true;
	}

	void Board::implodeLine(size_t y) {
		bool hasImmediateExplosion = false;
		bool hasDelayedExplosion = false;
		for (size_t i = 0; i < _blocks.size(); i++) {
			sb::Vector2i boardPos = worldToBoardPosition(_blocks[i].getPosition());
			if (boardPos.y == y) {
				bool isAboveOccupied = isOccupied(sb::Vector2i(boardPos.x, boardPos.y + 1));
				hasImmediateExplosion = hasImmediateExplosion || isAboveOccupied;
				hasDelayedExplosion = hasDelayedExplosion || !isAboveOccupied;
				float duration = isAboveOccupied ? 0.01f : 0.8f;
				_blocks[i].die(duration);
			}
		}

		if (hasImmediateExplosion)
			_explosionSound.play(0.01f);
		if (hasDelayedExplosion)
			_explosionSound.play(0.8f);
	}

	void Board::dropBlocksAboveLine(int y) {
		for (size_t i = 0; i < _blocks.size(); i++) {
			sb::Vector2i boardPos = worldToBoardPosition(_blocks[i].getPosition());
			if (boardPos.y > y)
				bounceBy(_blocks[i], sb::Vector2i(0, -1));
		}
	}

	void Board::clearLine(size_t y) {
		implodeLine(y);
		dropBlocksAboveLine(y);
		_linesCleared++;
	}

	void Board::clearLines() {
		for (int y = 0; y < _boardSize.y; y++) {
			while (isLineFull(y))
				clearLine(y);
		}
	}

	void Board::drop(Tetromino& tetromino) {
		float effectSeconds = 0.2f;
		driftBy(tetromino, sb::Vector2i(0, -1), effectSeconds);
		if (isInFinalPosition(tetromino))
			tetromino.playCollisionEffect(effectSeconds * 0.7f);

		if (isInvalid(tetromino)) {
			driftBy(tetromino, sb::Vector2i(0, 1));
			freeze(tetromino);
			clearLines();
			createRandomTetromino();
		}
	}

	void Board::step(Tetromino& tetromino, float ds) {
		_secondsSinceLastStep += ds;

		while (_secondsSinceLastStep >= _stepIntervalInSeconds) {
			if (_isAutodropEnabled)
				drop(tetromino);
			_secondsSinceLastStep -= _stepIntervalInSeconds;
		}
	}

	bool Board::isReachable(Tetromino& tetromino, const sb::Vector2f& targetPos, const sb::Vector2i& dir) {
		sb::Vector2i boardPosTo = worldToBoardPosition(targetPos);
		Tetromino testTetromino = tetromino;

		while (isValid(testTetromino)) {
			sb::Vector2i pos = worldToBoardPosition(testTetromino.getPosition());
			if (pos == boardPosTo)
				return true;
			moveBy(testTetromino, dir);
		}

		return false;
	}

	bool Board::isReachable(Tetromino& tetromino, const sb::Vector2f& targetPos) {
		static const std::vector<sb::Vector2i> directions = { sb::Vector2i(-1, 0), sb::Vector2i(1, 0), sb::Vector2i(0, -1) };

		for (size_t i = 0; i < directions.size(); i++) {
			if (isReachable(tetromino, targetPos, directions[i]))
				return true;
		}

		return false;
	}

	bool Board::isGarbage(const Block& block) {
		return block.getState() == Block::State::Garbage;
	}

	bool Board::isTetrominoGarbage(Tetromino& tetromino) {
		return tetromino.getState() == Tetromino::State::Garbage;
	}

	void Board::dispose() {
		_blocks.erase(std::remove_if(_blocks.begin(), _blocks.end(), isGarbage), _blocks.end());
		_dyingTetrominoes.erase(std::remove_if(_dyingTetrominoes.begin(), _dyingTetrominoes.end(), isTetrominoGarbage), _dyingTetrominoes.end());
	}

	void Board::bounceBy(Block& block, const sb::Vector2i& translation, float duration) {
		sb::Vector2i position = worldToBoardPosition(block.getPosition());
		position += translation;
		block.getEffects().bounceTo(boardToWorldPosition(position), block, duration);
	}

	int Board::getLowestLineWithBlocks() {
		int lowest = -1;
		for (size_t i = 0; i < _blocks.size(); i++) {
			sb::Vector2i boardPosition = worldToBoardPosition(_blocks[i].getPosition());
			if (boardPosition.y < lowest || lowest < 0)
				lowest = boardPosition.y;
		}

		return lowest;
	}

	bool Board::isInFinalPosition(Tetromino& tetromino) {
		if (isInvalid(tetromino))
			return false;

		Tetromino testTetromino = tetromino;
		driftBy(testTetromino, sb::Vector2i(0, -1));
		return isInvalid(testTetromino);
	}

	Board::Board(const sb::Vector2i& boardSize)
		: _batch(1024), _boardSize(boardSize), _grid(boardSize, 0.01f), _stripes(boardSize.x, 0.03f),
		_border(0.005f, boardSize.y / (float)boardSize.x, true), _isGridEnabled(false), _hasTetromino(false),
		_stepIntervalInSeconds(0.5f), _secondsSinceLastStep(0), _isFull(false), _linesCleared(0), _isAutodropEnabled(true)
	{
		_stripes.setScale(1, boardSize.y / (float)boardSize.x);
		_explosionSound.loadFromAssetPool("Sounds/Explosion.wav");
		_popSound.loadFromAssetPool("Sounds/Grab.wav");
		_spinSound.loadFromAssetPool("Sounds/Rotate.wav");
		_spinSound.setPlaybackInterval(0.1f);
	}

	sb::Vector2f Board::getSize() const {
		float inverseAspect = _boardSize.y / float(_boardSize.x);
		return sb::Vector2f(getScale().x * 1.f, getScale().y * inverseAspect);
	}

	sb::FloatRect Board::getBounds() {
		sb::Vector2f size = getSize();
		sb::FloatRect bounds(-size.x / 2, -size.y / 2, size.x, size.y);

		return getTransform() * bounds;
	}

	void Board::die() {
		_hasTetromino = false;
		_isFull = true;
	}

	void Board::createBlock(char type, const sb::Vector2i& position) {
		Block block(type);
		block.setPosition(boardToWorldPosition(position));
		block.setScale(getCellSize());
		_blocks.push_back(block);
	}

	void Board::createTetromino(char type, const sb::Vector2i& boardPosition) {
		Tetromino tetromino(type);
		createTetromino(tetromino, boardPosition);
	}

	void Board::createTetromino(char type) {
		Tetromino tetromino(type);
		sb::Vector2i boardPosition(_boardSize.x / 2 - (_boardSize.x % 2 ? 0 : 1),
			_boardSize.y - tetromino.getBlockBounds().top());
		createTetromino(tetromino, boardPosition);
	}

	void Board::driftTetrominoTo(sb::Vector2f pos, float effectSeconds) {
		sb::Vector2f previousPos = _tetromino.getPosition();
		sb::Vector2f newPos = getTransform() * pos;
		newPos.y = std::min(previousPos.y, newPos.y);

		if (isReachable(_tetromino, newPos)) {
			sb::Vector2i boardPos = worldToBoardPosition(newPos);
			sb::Vector2f worldPos = boardToWorldPosition(boardPos);
			_tetromino.getEffects().driftTo(worldPos, _tetromino, effectSeconds);
		}
	}

	void Board::driftTetrominoBy(const sb::Vector2i& translation) {
		sb::Vector2f cellSize = getCellSize();
		sb::Vector2f worldTranslation(translation.x * cellSize.x, translation.y * cellSize.y);
		sb::Vector2f end = _tetromino.getPosition() + worldTranslation;
		float effectSeconds = 0.2f;
		driftTetrominoTo(end, effectSeconds);

		if (isInFinalPosition(_tetromino))
			_tetromino.playCollisionEffect(effectSeconds * 0.7f);
	}

	void Board::spinTetromino() {
		_tetromino.getEffects().spinBy(-90 * sb::ToRadian, _tetromino, 0.75f);

		if (isInvalid(_tetromino))
			_tetromino.getEffects().spinBy(90 * sb::ToRadian, _tetromino);
		else
			_spinSound.play();
	}

	void Board::popTetromino() {
		_tetromino.getEffects().pop(_tetromino);
		_popSound.play();
	}

	void Board::dropTetromino() {
		drop(_tetromino);
	}

	void Board::hardDropTetromino() {
		Tetromino projection = computeProjection();
		_secondsSinceLastStep = 0;
		float effectSeconds = 0.5f;
		_tetromino.getEffects().bounceTo(projection.getPosition(), _tetromino, effectSeconds);
		_tetromino.playCollisionEffect(effectSeconds * 0.6f);
	}

	void Board::dropBlocks() {
		for (size_t i = 0; i < _blocks.size(); i++)
			bounceBy(_blocks[i], sb::Vector2i(0, -1));
	}

	void Board::implodeBlocks(float duration) {
		for (size_t i = 0; i < _blocks.size(); i++)
			_blocks[i].die(duration);
	}

	void Board::clearLowestLineWithBlocks() {
		int y = getLowestLineWithBlocks();
		if (y >= 0)
			clearLine(y);
	}

	Tetromino Board::computeProjection() {
		Tetromino projection(_tetromino);
		projection.getEffects().stop();
		while (!isInvalid(projection))
			moveBy(projection, sb::Vector2i(0, -1));
		moveBy(projection, sb::Vector2i(0, 1));

		return projection;
	}

	void Board::updateEntities(float ds) {
		dispose();

		for (size_t i = 0; i < _blocks.size(); i++)
			_blocks[i].update(ds);

		if (_hasTetromino)
			_tetromino.update(ds);
		else
			createRandomTetromino();

		for (size_t i = 0; i < _dyingTetrominoes.size(); i++)
			_dyingTetrominoes[i].update(ds);
	}

	void Board::updateSounds(float ds) {
		_explosionSound.update(ds);
		_popSound.update(ds);
		_spinSound.update(ds);
	}

	void Board::update(float ds) {
		updateEntities(ds);
		updateSounds(ds);

		if (!_isFull) {
			step(_tetromino, ds);
		}
	}

	void Board::draw(sb::DrawTarget& target, sb::DrawStates states) {
		states.transform *= getTransform();

		target.draw(_stripes, states);

		if (_isGridEnabled)
			target.draw(_grid, states);

		for (size_t i = 0; i < _blocks.size(); i++)
			_batch.draw(_blocks[i], states);
		target.draw(_batch);
		for (size_t i = 0; i < _blocks.size(); i++)
			_blocks[i].drawExplosion(target, states);

		if (_hasTetromino) {
			Tetromino projection = computeProjection();
			projection.setColor(sb::Color(1, 1, 1, 0.25f));
			_batch.draw(projection, states);
			_batch.draw(_tetromino, states);
			target.draw(_batch);
			_tetromino.drawCollisionEffect(target, states);
			_tetromino.drawBubbleEffect(target, states);
		}

		for (size_t i = 0; i < _dyingTetrominoes.size(); i++) {
			_dyingTetrominoes[i].drawCollisionEffect(target, states);
			_dyingTetrominoes[i].drawBubbleEffect(target, states);
		}

		target.draw(_border, states);
	}
}
