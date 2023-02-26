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
	Block();//���캯��
	void drop();//����
	void moveLeftRight(int offset);//ƽ��
	void rotate();//��ת
	void draw(int leftedge, int topedge);//���Ʒ���
	static IMAGE** getImages();
	Block& operator=(const Block& other);
	bool blockInMap(const vector<vector<int>>& map);
	void solidify(vector<vector<int>>& map);
	int getBlockType();
private:
	int BlockType;//��������
	Point smallBlocks[4];//����ĸ�����λ��
	IMAGE* img;
	static IMAGE* imgs[7];
	static int size;
};

