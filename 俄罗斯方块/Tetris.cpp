#include "Tetris.h"
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "Block.h"
#include <mmsystem.h>
#include <fstream>
#include <iostream>
#pragma comment(lib,"winmm.lib")

#define MAX_LEVEL 5
#define RECORDER_FILE "recorder.txt"
const int SPEED_NORMAL[5] = { 500,400,300,200,100 };
const int SPEED_QUICK = 30;

Tetris::Tetris(int rows, int cols, int left, int top, int blockSize)
{
	this->rows = rows;
	this->cols = cols;
	this->leftEdge = left;
	this->topEdge = top;
	this->blockSize = blockSize;
	for (int i = 0; i < rows; i++) {
		vector<int> mapRow;
		for (int j = 0; j < cols; j++) {
			mapRow.push_back(0);
		}
		map.push_back(mapRow);
	}
}

void Tetris::init()
{
	mciSendString("play res/bg.mp3 repeat", 0, 0, 0);
	delay = SPEED_NORMAL[level];
	srand(time(NULL));
	initgraph(938, 896);//创建游戏窗口
	loadimage(&imgBg,"res/bg2.png");//加载背景图片
	loadimage(&imgOver, "res/over.png");
	loadimage(&imgWin, "res/win.png");
	score = 0;
	level = 1;
	lineCount = 0;
	gameOver = false;
	highestScore = 0;
	ifstream file(RECORDER_FILE);
	if (!file.is_open()) {
		std::cout << RECORDER_FILE << "打开失败" << endl;
		highestScore = 0;
	}
	else {
		file >> highestScore;
	}
	file.close();
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			map[i][j] = 0;
}

void Tetris::playgame()
{
	init();
	nextBlock = new Block;
	curBlock = nextBlock;
	nextBlock = new Block;
	int timer = 0;
	while (1) {
		if (gameOver) {
			saveScore();
			displayOver();
			system("pause");
		}
		keyevent();
		timer += getdelay();
		if (timer > delay) {
			timer = 0;
			drop();
			update = true;
		}
		if (update) {
			update = false;
			updatewindow();
			clearline();
		}
	}
}

void Tetris::keyevent()
{
	bool update;
	bool rotateFlag = false;
	int dx = 0;
	unsigned char ch;
	if (_kbhit()) {
		ch = _getch();//向上方向键返回224,72，向下返回224,80，向左返回224,75，向右返回224,77
		if (ch == 224) {
			update = true;
			ch = _getch();
			switch (ch)
			{
			case 72:
				rotateFlag = true;
				break;
			case 80:
				delay = SPEED_QUICK;
				break;
			case 75:
				dx = -1;
				break;
			case 77:
				dx = 1;
				break;
			default:
				break;
			}
		}
	}
	if (rotateFlag) {//旋转方块
		if (curBlock->getBlockType() == 7)
			return;
		bakBlock = *curBlock;
		curBlock->rotate();
		if (!curBlock->blockInMap(map))
			*curBlock = bakBlock;
	}
	if (dx != 0) {//平移方块
		moveLeftRight(dx);
	}
}

void Tetris::updatewindow()
{
	BeginBatchDraw();
	putimage(0, 0, &imgBg);//绘制背景图
	IMAGE** imgs = Block::getImages();
	for (int i = 0; i < map.size(); i++) {
		for (int j = 0; j < map[0].size(); j++) {
			if (map[i][j] == 0) continue;
			int x = j * blockSize + leftEdge;
			int y = i * blockSize + topEdge;
			putimage(x, y, imgs[map[i][j] - 1]);
		}
	}
	curBlock->draw(leftEdge, topEdge);
	nextBlock->draw(689, 130);
	drawScore();
	EndBatchDraw();
} 

int Tetris::getdelay()
{
	static unsigned long long lastTime = 0;
	unsigned long long currentTime = GetTickCount();
	if (lastTime == 0) {
		lastTime = currentTime;
		return 0;
	}
	int ret = currentTime - lastTime;
	lastTime = currentTime;
	return ret;
}

void Tetris::drop()
{
	bakBlock = *curBlock;
	curBlock->drop();
	if (!curBlock->blockInMap(map)) {
		bakBlock.solidify(map);
		delete curBlock;
		curBlock = nextBlock;
		nextBlock = new Block;
		update = true;
		checkOver();
	}
	delay = SPEED_NORMAL[level];
}

void Tetris::clearline()
{
	int lines = 0;
	int k = rows - 1;
	for (int i = rows - 1; i >= 0; i--) {
		int cnt = 0;
		for (int j = 0; j < cols; j++) {
			if (map[i][j])
				cnt++;
			map[k][j] = map[i][j];
		}
		if (cnt < cols) {
			k--;
		}
		else lines++;
	}
	if (lines > 0) {
		int addScore[4] = { 10,30,60,80 };//计算得分
		score += addScore[lines - 1];
		mciSendString("play res/xiaochu1.mp3", 0, 0, 0);
		update = true;
		level = score / 100 + 1;
		lineCount += lines;
		if (level > MAX_LEVEL) gameOver = true;
	}
}

void Tetris::moveLeftRight(int offset)
{
	bakBlock = *curBlock;
	curBlock->moveLeftRight(offset);
	if (!curBlock->blockInMap(map))
		*curBlock = bakBlock;
}

void Tetris::drawScore()
{
	char scoreText[32];
	sprintf_s(scoreText, sizeof(scoreText), "%d", score);
	setcolor(RGB(180, 180, 180));
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 60;
	f.lfWidth = 30;
	f.lfQuality = ANTIALIASED_QUALITY;//抗锯齿
	strcpy_s(f.lfFaceName, sizeof(f.lfFaceName), _T("Segoe UI Black"));//设置字体
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	outtextxy(670, 727, scoreText);
	sprintf_s(scoreText, sizeof(scoreText), "%d", lineCount);
	gettextstyle(&f);
	int xPos = 224 - f.lfWidth * strlen(scoreText);
	outtextxy(xPos, 817, scoreText);
	sprintf_s(scoreText, sizeof(scoreText), "%d", level);
	outtextxy(194, 727, scoreText);
	sprintf_s(scoreText, sizeof(scoreText), "%d", highestScore);
	outtextxy(670, 817, scoreText);
}

void Tetris::checkOver()
{
	if (curBlock->blockInMap(map) == false) 
		gameOver = true;
}

void Tetris::saveScore()
{
	if (score > highestScore) {
		highestScore = score;
		ofstream file(RECORDER_FILE);
		file << highestScore;
		file.close();
	}
}

void Tetris::displayOver()
{
	mciSendString("stop res/bg2.mp3", 0, 0, 0);
	if (level <= MAX_LEVEL) {
		putimage(262, 361, &imgOver);
		mciSendString("play res/over.mp3", 0, 0, 0);
	}
	else {
		putimage(262, 361, &imgWin);
		mciSendString("play res/win.mp3", 0, 0, 0);
	}
}
