class Node {
	vector<Drawable*> _drawables;
	vector<Node*> _children;
	
	void draw(const Transform& transform) {
		transform *= getTransform();
		drawChildren(transform);
		drawDrawables(transform);
		drawSelf(transform);
	}
	
	void update() {
		updateChildren();
		updateSelf();
	}
	
	void addDrawable(Drawable* ownedDrawable) { _drawables.push_back(ownedDrawable); }
		
	void addChild(Node* ownedNode) { _nodes.push_back(ownedNode); }
}

class Block : public Node {
	Block(GameAssets& gameAssets) {
		Texture& texture = assets.getTexture("Textures/GreenBlock.png")
		Sprite* sprite = new Sprite(texture);
		addDrawable(sprite);
	}
	
	void updateSelf() override {
		updateBlockLogic();
	}
}

class Board : public Node {	
	Board {
		for (...)
			addChild(new Block(x, y));
	}
	
	void drawSelf(const Transform& transform) override {
		drawBackground(transform);
	}
	
	void updateSelf() override {
		updateBoardLogic();
	}
}

int main() {
	Board board;
	board.update();
	board.draw(Transform());
}