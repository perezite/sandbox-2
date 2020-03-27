struct Vector2D : MR_TYPE(Vector2D) {
	MR_NAME(Vector2D)
	MR_FIELD(float, x);
	MR_FIELD(float, y);
};

class GameEntity : MR_TYPE(GameEntity) {
	MR_NAME(GameEntity)
	MR_FIELD(Vector2D, _position);
public:
	const Vector2D& getPosition() const { return _position; }
	void setPosition(const Vector2D& position) { _position = position; }
};

class SpaceShip : public GameEntity { 
	MR_NAME(SpaceShip)	
	MR_FIELD(int, _health);
public:
	int getHealth() { return _health; }
	void setHealth(int health) { _health = health; }
};

class BattleCruiser : public GameEntity {
	MR_NAME(SpaceShip)
};

class Scene : MR_TYPE(Scene) {
	MR_NAME(Scene)
	MR_FIELD(vector<GameEntity*>, _entities);
public:
	void addEntity(GameEntity& entity) { _entities.push_back(&entity); }
};

void main() {
	// create scene
	SpaceShip ship; ship.setHealth(42); ship.setPosition(Vector2D(1, 2));
	BattleCruiser cruiser; cruiser.setPosition(Vector2D(3, 4));
	Scene scene; scene.addEntity(ship); scene.addEntity(cruiser);

	// write scene to file
	ofstream ofs("savegame.txt");
	TextWriter::write(scene, ofs);	

	// reload scene from file
	Scene reloadedScene;
	ifstream ifs("savegame.txt");
	TextReader::read(reloadedScene, ifs);	
	
	// write result to console
	TextWriter:::write(reloadedScene, cout);	
}