	#define MR_FIELD(a, b) a b
	#define MR_TYPE(x)

	namespace mr {
		template <class T> class Type { };
	}

	struct Vector2D : public mr::Type<Vector2D> {
		MR_TYPE(Vector2D)
		MR_FIELD(float, x);
		MR_FIELD(float, y);
	};

	class GameEntity : public mr::Type<GameEntity> {
		MR_TYPE(GameEntity)
		MR_FIELD(Vector2D, _position);
	public:
		const Vector2D& getPosition() const { return _position; }
		void setPosition(float x, float y) { _position.x = x; _position.y = y; }
	};

	class SpaceShip : public GameEntity { 
		MR_TYPE(SpaceShip)	
		MR_FIELD(int, _health);
	public:
		int getHealth() { return _health; }
		void setHealth(int health) { _health = health; }

	};

	class BattleCruiser : public GameEntity {
		MR_TYPE(SpaceShip)
	};

	class Scene {
		MR_TYPE(Scene)
		MR_FIELD(vector<GameEntity*>, _gameEntities);
	public:
		void addEntity(GameEntity& entity) { _gameEntities.push_back(&entity); }
	};

	void main() {
		// create scene
		SpaceShip ship;
		ship.setHealth(42); ship.setPosition(1, 1);
		BattleCruiser cruiser;
		cruiser.setPosition(2, 2);
		Scene scene;
		scene.addEntity(ship); scene.addEntity(cruiser);

		// write scene to output file stream
		ofstream ofs("savegame.txt");
		//TextWriter::write(scene, ofs);	

		// reload scene from input file stream and write result to standard console output
		Scene reloadedScene;
		ifstream ifs("savegame.txt");
		//TextReader::read(reloadedScene, ifs);	
		//TextWriter:::write(reloadedScene, cout);	
	}