#include <iostream>
#include <conio.h>
#include <random>
#include <time.h>

class Game {
public:
	Game();
	~Game();

	void Run();

private:
	enum {
		SHAPE_I,
		SHAPE_O,
		SHAPE_S,
		SHAPE_Z,
		SHAPE_J,
		SHAPE_L,
		SHAPE_T,
		SHAPE_MAX
	};


	struct SHAPE {
		int width;
		int height;
		int** shape;
	};

	struct MINO {
		int x, y;
		SHAPE shape;
	};

	MINO mino;

	void DrawScreen();
	void InitMino(); // minoÇÃèâä˙âª
	bool MinoIntersectField(); // MinoÇÃè’ìÀîªíË

	SHAPE* shapes;
	int** mField;
	int** mScreen;
	const int SHAPE_WIDTH_MAX;
	const int SHAPE_HEIGHT_MAX;
	const int FILED_WIDTH;
	const int FILED_HEIGHT;

	const int FPS;
	const int INTERVAL;

	bool GameOverFlg;

	std::mt19937 random_generator;
};