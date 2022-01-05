#include "Game.hpp"

Game::Game()
	:FILED_WIDTH(10),
	FILED_HEIGHT(20),
	SHAPE_HEIGHT_MAX(4),
	SHAPE_WIDTH_MAX(4),
	random_generator((int)time(0)),
	FPS(1),
	INTERVAL(1000 / FPS),
	GameOverFlg(false)
{
	shapes = new SHAPE[SHAPE_MAX];

	int shape_data[][4][4] = {
		{ // SHAPE_I
			{0, 0, 0, 0},
			{1, 1, 1, 1},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},
		{ // SHAPE_O
			{1, 1, 0, 0},
			{1, 1, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},
		{ // SHAPE_S
			{0, 1, 1, 0},
			{1, 1, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},
		{ // SHAPE_Z
			{1, 1, 0, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},
		{ // SHAPE_J
			{1, 0, 0, 0},
			{1, 1, 1, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},
		{ // SHAPE_L
			{0, 0, 1, 0},
			{1, 1, 1, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},
		{ // SHAPE_T
			{0, 1, 0, 0},
			{1, 1, 1, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		}
	};
	int shape_size_array[][2] = {
		{4, 4},
		{2, 2},
		{3, 3},
		{3, 3},
		{3, 3},
		{3, 3},
		{3, 3}
	};
	for (int i = 0; i < SHAPE_MAX; i++) {
		SHAPE shape;
		shape.width = shape_size_array[i][0];
		shape.height = shape_size_array[i][1];
		shape.shape = new int* [4];
		for (int j = 0; j < 4; j++) {
			shape.shape[j] = new int[4];
			for (int k = 0; k < 4; k++) {
				shape.shape[j][k] = shape_data[i][j][k];
			}
		}
		shapes[i] = shape;
	}

	mField = new int* [FILED_HEIGHT];
	mScreen = new int* [FILED_HEIGHT];
	for (int i = 0; i < FILED_HEIGHT; i++) {
		mField[i] = new int[FILED_WIDTH];
		mScreen[i] = new int[FILED_WIDTH];
		for (int j = 0; j < FILED_WIDTH; j++) {
			mField[i][j] = 0;
			mScreen[i][j] = 0;
		}
	}
}

Game::~Game()
{

}

void Game::InitMino()
{
	mino.shape = shapes[random_generator() % SHAPE_MAX];
	mino.x = (FILED_WIDTH - mino.shape.width) / 2;
	mino.y = 0;
}

bool Game::MinoIntersectField()
{
	bool OufOfField = false;
	bool CollideWithField = false;
	for (int y = 0; y < mino.shape.height; y++) {
		for (int x = 0; x < mino.shape.width; x++) {
			// minoのローカル座標系について
			if (mino.shape.shape[y][x]) {  // minoの各ブロックについて、範囲内にあるかチェック
				if ((mino.y + y < 0) || (mino.y + y >= FILED_HEIGHT) ||
					(mino.x + x < 0) || (mino.x + x >= FILED_WIDTH)) {
					return true;
				} else if (mField[mino.y + y][mino.x + x]) {
				// すでに積み上げられたブロックと衝突したか?
					return true;
				}
			}
		}
	}

	return false;
}

void Game::DrawScreen()
{
	// screenにfieldをコピー
	for (int i = 0; i < FILED_HEIGHT; i++) {
		for (int j = 0; j < FILED_WIDTH; j++) {
			mScreen[i][j] = mField[i][j];
		}
	}

	// MINOを追加
	for (int y = 0; y < mino.shape.height; y++) {
		for (int x = 0; x < mino.shape.width; x++) {
			if (mino.shape.shape[y][x]) {
				mScreen[mino.y + y][mino.x + x] = 1;
			}
		}
	}

	// screen に表示
	system("cls");
	for (int y = 0; y < FILED_HEIGHT; y++) {
		std::cout << "□"; // 左端の壁
		for (int x = 0; x < FILED_WIDTH; x++) {
			// mScreenの値が1ならば描画
			std::cout << ((mScreen[y][x]) ? "■" : "　"); // ブロックがあればそれを描画
		}
		std::cout << "□"; // 右端の壁
		std::cout << std::endl;
	}
	for (int x = 0; x < FILED_WIDTH + 2; x++) {
		std::cout << "□"; // 床
	}

}

void Game::Run()
{
	InitMino();

	clock_t lastClock = clock();
	while (1) {
		clock_t newClock = clock();
		if (newClock >= lastClock + INTERVAL) { // 時刻が更新されたら

			MINO lastMINO = mino;
			mino.y++; // テトリミノを落下
			if (MinoIntersectField()) { // 地面と衝突したら

				mino = lastMINO;

				// 地面と同化する
				for (int y = 0; y < mino.shape.height; y++) {
					for (int x = 0; x < mino.shape.width; x++) {
						if (mino.shape.shape[y][x]) {
							mField[mino.y + y][mino.x + x] = 1; // 
						}
					}
				}

				// 1行揃ったら消去
				for (int y = 0; y < FILED_HEIGHT; y++) {
					bool continued = true;
					for (int x = 0; x < FILED_WIDTH; x++) {
						if (!mField[y][x]) {
							continued = false;
							break;
						}
					}
					if (continued) { // 1行そろったら
						// 1行削除
						for (int x = 0; x < FILED_WIDTH; x++) {
							mField[y][x] = 0;
						}
						// 下にスライドさせる
						for (int y2 = y; y2 >= 1; y2--) {
							for (int x = 0; x < FILED_WIDTH; x++) {
								mField[y2][x] = mField[y2 - 1][x];
								mField[y2 - 1][x] = 0;
							}
						}
					}
				}

				// ゲームオーバー処理
				for (int x = 0; x < FILED_WIDTH; x++) { // fieldの一番上までMINOが積みあがったら
					if (mField[0][x]) {
						GameOverFlg = true;
					}
				}

				if (GameOverFlg) {
					break;
				}
				InitMino(); // 新しいMINO作成
			}
			DrawScreen();
			lastClock = newClock;
		}

		DrawScreen();
		MINO LastMINO = mino; // 動く前のMINO
		if (_kbhit()) { // キーボードが押されたら
			switch (_getch()) {
			case 'w':
				mino.y--;
				break;
			case 's':
				mino.y++;
				break;
			case 'a':
				mino.x--;
				break;
			case 'd':
				mino.x++;
				break;
			default: // なにかキーを押されたら回転
				// 初期化処理
				MINO newMINO;
				newMINO.x = mino.x;
				newMINO.y = mino.y;
				newMINO.shape.width = mino.shape.width;
				newMINO.shape.height = mino.shape.height;
				newMINO.shape.shape = new int* [newMINO.shape.height];
				for (int i = 0; i < newMINO.shape.height; i++) {
					newMINO.shape.shape[i] = new int[newMINO.shape.width];
				}
				// 回転処理
				for (int y = 0; y < mino.shape.height; y++) {
					for (int x = 0; x < mino.shape.width; x++) {
						newMINO.shape.shape[mino.shape.width - 1 - x][y] = mino.shape.shape[y][x];
					}
				}
				mino = newMINO;
			}
		}
		if (MinoIntersectField()) {
			mino = LastMINO; // 衝突していたら動かない
		}

		if (GameOverFlg) {
			break;
		}
	}

	std::cout << "\nGAME OVER" << std::endl;
}
