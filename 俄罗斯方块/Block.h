#pragma once
#include <graphics.h>
#include <vector>
using namespace std;

struct Point {
	int row;
	int col;
};

class Block
{
public:
	Block();//构造函数
	void drop();//下落
	void moveLeftRight(int offset);//平移
	void rotate();//旋转
	void draw(int leftedge, int topedge);//绘制方块
	static IMAGE** getImages();
	Block& operator=(const Block& other);
	bool blockInMap(const vector<vector<int>>& map);
	void solidify(vector<vector<int>>& map);
	int getBlockType();
private:
	int BlockType;//方块类型
	Point smallBlocks[4];//标记四个方块位置
	IMAGE* img;
	static IMAGE* imgs[7];
	static int size;
};

