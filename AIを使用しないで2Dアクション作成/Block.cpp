#include "Block.h"
Block::Block(float startX, float startY, float w, float h) {
	x = startX;
	y = startY;
	width = w;
	height = h;
	type = BlockType::Normal;
}

void Block::Update() {

}

void Block::Draw()const {
	switch (type)
	{
	case BlockType::Normal:
		DrawBox(x, y, x + BLOCK_SIZE, y + BLOCK_SIZE, Col.GetGra(), TRUE);
		DrawBox(x, y, x + BLOCK_SIZE, y + BLOCK_SIZE, Col.GetBla(), FALSE);
		break;
	case BlockType::Gravity:
		DrawBox(x, y, x + BLOCK_SIZE, y + BLOCK_SIZE, Col.GetGre(), TRUE);
		break;
	case BlockType::Move:
		DrawBox(x, y, x + BLOCK_SIZE, y + BLOCK_SIZE, Col.GetWhi(), TRUE);
		break;
	case BlockType::Max:
		break;
	case BlockType::NoType:
		break;
	default:
		break;
	}
}