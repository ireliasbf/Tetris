#pragma once
#include <vector>
#include <graphics.h>
#include "Block.h"
using namespace std;

class Tetris
{
public:
	Tetris(int rows, int cols, int left, int top, int blockSize);//构造函数
	void init();//初始化
	void playgame();//开始游戏
private:
	void keyevent();//用户输入
	void updatewindow();//更新游戏画面
	int getdelay();//返回距离上次调用该函数经过多久
	void drop();
	void clearline();//消除之后清行
	void moveLeftRight(int offset);
	void drawScore();
	void checkOver();
	void saveScore();
	void displayOver();
	int delay;
	bool update = false;
	vector<vector<int>> map;
	int rows;
	int cols;
	int leftEdge;
	int topEdge;
	int blockSize;
	int score = 0;
	int level = 1;
	int lineCount = 0;
	int highestScore = 0;
	bool gameOver = false;
	IMAGE imgBg;
	IMAGE imgOver;
	IMAGE imgWin;
	Block* curBlock;
	Block* nextBlock;
	Block bakBlock;
};

