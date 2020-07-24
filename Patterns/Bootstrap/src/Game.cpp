#include "Board.h"
#include "Backdrop.h"
#include "Music.h"
#include "Input.h"
#include "Stopwatch.h"
#include <vector>
#include <algorithm>
using namespace blocks;

namespace game {
	static float lastElapsed = 0;

	sb::Vector2i getWindowSize(size_t width, float inverseAspect) {
		return sb::Vector2i(width, int(inverseAspect * width));
	}

	float getSeconds() {
		static sb::Stopwatch sw;
		return sw.getElapsedSeconds();
	}

	float getDeltaSeconds() {
		float elapsed = getSeconds();
		float delta = elapsed - lastElapsed;
		lastElapsed = elapsed;
		return delta;
	}

	void resetDeltaSeconds() {
		lastElapsed = getSeconds();
	}

	void adjustCameraToBoard(sb::Camera& camera, const Board& board) {
		float cameraAspect = camera.getAspectRatio();
		const sb::Vector2f& boardSize = board.getSize();
		float boardAspect = boardSize.x / boardSize.y;

		if (boardAspect > cameraAspect)
			camera.setWidth(board.getSize().x);
		else
			camera.setHeight(board.getSize().y);
	}

	sb::Vector2f getSwipe(sb::Window& window, float ds) {
		static sb::Vector2f start;
		static float secondsSinceStart;
		static bool swiping = false;
		const float maxSwipeSeconds = 0.75f;

		bool swipeStarting = sb::Input::isTouchGoingDown(1);
		bool swipeEnding = swiping && !sb::Input::isTouchGoingDown(1) && !sb::Input::isTouchDown(1);

		if (swipeStarting) {
			start = sb::Input::getTouchPosition(window);
			swiping = true;
			secondsSinceStart = 0;
			return sb::Vector2f(0, 0);
		}
		else if (swipeEnding) {
			swiping = false;
			if (secondsSinceStart <= maxSwipeSeconds)
				return sb::Input::getTouchPosition(window) - start;
		}

		secondsSinceStart += ds;
		return sb::Vector2f(0, 0);
	}

	void touchInput(sb::Window& window, Board& board, float ds) {
		static float secondsSinceLastTouch = 1;
		static sb::Vector2f touchOffset;
		secondsSinceLastTouch += ds;

		if (sb::Input::isTouchGoingDown(1)) {
			touchOffset = board.getTetromino().getPosition() - sb::Input::getTouchPosition(window);
			if (secondsSinceLastTouch >= 1)
				board.popTetromino();
			secondsSinceLastTouch = 0;
		}
		if (sb::Input::isTouchDown(1))
			board.driftTetrominoTo(sb::Input::getTouchPosition(window) + touchOffset);
		if (getSwipe(window, ds).y > 0.05f * window.getInverseAspect())
			board.spinTetromino();
	}

	void input(sb::Window& window, Board& board, float ds) {
		if (sb::Input::isKeyGoingDown(sb::KeyCode::Up))
			board.spinTetromino();
		if (sb::Input::isKeyGoingDown(sb::KeyCode::p))
			board.popTetromino();
		if (sb::Input::isKeyGoingDown(sb::KeyCode::Down))
			board.driftTetrominoBy(sb::Vector2i(0, -1));
		if (sb::Input::isKeyGoingDown(sb::KeyCode::Left))
			board.driftTetrominoBy(sb::Vector2i(-1, 0));
		if (sb::Input::isKeyGoingDown(sb::KeyCode::Right))
			board.driftTetrominoBy(sb::Vector2i(1, 0));
		if (sb::Input::isKeyGoingDown(sb::KeyCode::Space))
			board.hardDropTetromino();

		touchInput(window, board, ds);
	}

	void updateScene(Board& board) {
		if (board.isDead()) {
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "You lost the game", "You lost the game, press OK to try again", NULL);
			resetDeltaSeconds();
			board = Board(board.getBoardSize());
		}
	}

	void game() {
		sb::Window window(getWindowSize(400, 3.f / 2.f));
		Backdrop backdrop;
		Board board(sb::Vector2i(10, 14));
		sb::Music music;

		music.loadFromAsset("Music/BackgroundMusic.ogg");
		music.setLooping(true);
		music.play();

		adjustCameraToBoard(window.getCamera(), board);

		while (window.isOpen()) {
			float ds = getDeltaSeconds();
			sb::Input::update();
			window.update();
			backdrop.update(window.getCamera());
			board.update(ds);
			updateScene(board);

			input(window, board, ds);

			window.clear(sb::Color(1, 1, 1, 1));
			window.draw(backdrop);
			sb::Transform test;
			window.draw(board, test);
			window.display();
		}
	}
}
