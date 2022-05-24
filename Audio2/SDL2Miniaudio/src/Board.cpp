#include "Board.h"
#include "Block.h"
#include "Window.h"
#include "Memory.h"
#include "Math.h"
#include "Rect.h"
#include "DrawTarget.h"
#include "VectorHelper.h"

using namespace sb;
using namespace std;

namespace blocks
{
    void Board::cureTetromino()
    {
        vector<Vector2i> positions; _tetromino->getTransformedSquarePositions(positions);
        _tetromino->getTransformedSquarePositions(positions);

        for (size_t i = 0; i < positions.size(); i++) {
            Block* block = new Block(_assets.blockTextureAtlas, _tetromino->getType());
            block->setPosition(toVector2f(positions[i]));
            _blocks.push_back(block);
        }
    }

    void Board::respawnTetromino()
    {
        if (_tetromino)
            cout << "respawn from position " << _tetromino->getPosition().x << " " << _tetromino->getPosition().y << endl;     
        safeDelete(_tetromino);
        
        _tetromino = new Tetromino(*this, _assets.squareTextureAtlas, BlockType::T);
        _tetromino->setPosition(5, 16);
        
        _collisionLogic.resetTetrominoData();
    }

    void Board::handleCollisions()
    {
        if (_collisionLogic.isTetrominoDead()) {
            cureTetromino();
            respawnTetromino();
        }

        if (_collisionLogic.isTetrominoStuck())
            cout << "Game over!!" << endl;
    }

    void Board::harddropTetromino()
    {
        while (!_collisionLogic.hasTetrominoCollision())
            _tetromino->translate(0, -1);
    }

    Board::Board(GameAssets& assets, size_t width, size_t height) : 
        _assets(assets), _size(width, height), _tetromino(NULL), 
        _collisionLogic(*this)
    { 
        _blocks.push_back(new Block(assets.blockTextureAtlas));
        _blocks[0]->setPosition(3, 14);
        //_blocks[0]->setPosition(0, 0);
        respawnTetromino();

        //_blocks[0]->setPosition(3, 3);
        //_tetromino.setPosition(1, 1);
    }

    Board::~Board() 
    {
        deleteAll(_blocks); 
        delete _tetromino;
    }

    void Board::getBlockPositions(std::vector<sb::Vector2i>& result)
    {
        result.clear(); result.reserve(_blocks.size());

        for (size_t i = 0; i < _blocks.size(); i++) {
            Vector2i blockPosition = toVector2i(_blocks[i]->getPosition());
            result.push_back(blockPosition);
        }
    }

    void Board::update(Window& window)
    {
        _tetromino->update();
        // _collisionLogic.update();
        // handleCollisions();
    }

    void Board::draw(DrawTarget& target, DrawStates states)
    {
        states.transform *= getTransform();
        target.draw(_tetromino, states);

        for (size_t i = 0; i < _blocks.size(); i++)
            _batch.draw(*_blocks[i], states);

        target.draw(_batch);
    }
}