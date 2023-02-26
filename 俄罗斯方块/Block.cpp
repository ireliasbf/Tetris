#include "Block.h"
#include <stdlib.h>

IMAGE* Block::imgs[7] = { NULL, };
int Block::size = 36;

Block::Block()
{
	if (imgs[0]==NULL) {//新出现的方块绘制
		IMAGE imgTmp;
		loadimage(&imgTmp, "res/tiles.png");
		SetWorkingImage(&imgTmp);
		for (int i = 0; i < 7; i++) {
			imgs[i] = new IMAGE;
			getimage(imgs[i], i * size, 0, size, size);
		}
		SetWorkingImage();
	}
	int blocks[7][4] = {
	1,3,5,7,//I型
	2,4,5,7,//Z1型
	3,5,4,6,//Z2型
	3,5,4,7,//T型
	2,3,5,7,//L型
	3,5,7,6,//J型
	2,3,4,5//田字型
	};
	BlockType = 1 + rand() % 7;//新出现的方块类型随机
	for (int i = 0; i < 4; i++) {//为方块每一个小块赋予位置信息
		int value = blocks[BlockType - 1][i];
		smallBlocks[i].row = value / 2;
		smallBlocks[i].col = value % 2;
	}
	img = imgs[BlockType - 1];
}

void Block::drop()
{
	for (int i = 0; i < 4; i++)
		smallBlocks[i].row++;
}

void Block::moveLeftRight(int offset)
{
	for (int i = 0; i < 4; i++) {
		smallBlocks[i].col += offset;
	}
}

void Block::rotate()
{
	Point p = smallBlocks[1];
	for (int i = 0; i < 4; i++) {
		Point tmp = smallBlocks[i];
		smallBlocks[i].col = p.col - tmp.row + p.row;
		smallBlocks[i].row = p.row + tmp.col - p.col;
	}
}

void Block::draw(int leftEdge, int topEdge)
{
	for (int i = 0; i < 4; i++) {
		int x = leftEdge + smallBlocks[i].col * size;
		int y = topEdge + smallBlocks[i].row * size;
		putimage(x, y, img);
	}
}

IMAGE** Block::getImages()
{
	return imgs;
}

Block& Block::operator=(const Block& other)
{
	if (this == &other) return *this;
	this->BlockType = other.BlockType;
	for (int i = 0; i < 4; i++)
		this->smallBlocks[i] = other.smallBlocks[i];
	return *this;
}

bool Block::blockInMap(const vector<vector<int>>& map)
{
	int rows = map.size();
	int cols = map[0].size();
	for (int i = 0; i < 4; i++) {
		if (smallBlocks[i].row < 0 || smallBlocks[i].row >= rows || smallBlocks[i].col < 0
			|| smallBlocks[i].col >= cols || map[smallBlocks[i].row][smallBlocks[i].col])
			return false;
	}
	return true;
}

void Block::solidify(vector<vector<int>>& map)
{
	for (int i = 0; i < 4; i++) {
		map[smallBlocks[i].row][smallBlocks[i].col] = BlockType;
	}
}

int Block::getBlockType()
{
	return BlockType;
}
