#pragma once
#include <vector>
#include <graphics.h>
#include "Block.h"
using namespace std;

class Tetris
{
public:
	Tetris(int rows, int cols, int left, int top, int blockSize);//���캯��
	void init();//��ʼ��
	void playgame();//��ʼ��Ϸ
private:
	void keyevent();//�û�����
	void updatewindow();//������Ϸ����
	int getdelay();//���ؾ����ϴε��øú����������
	void drop();
	void clearline();//����֮������
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

