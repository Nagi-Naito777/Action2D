#include "Block.h"
Block::Block(float startX, float startY, float w, float h, BlockType t) {
	x = startX;
	y = startY;
	width = w;
	height = h;
	type = t;
}

void Block::Update() {

}

void Block::Draw(float centerX, float centerY, float angle) const {
	float newX, newY;

	// 自分の座標(x, y)を回転させた先の座標(newX, newY)で計算
	GetRotatedPosition(centerX, centerY, x, y, &newX, &newY, angle);
	
	// 座標を整数に変換してから描画する(回転後のブロックのズレを無くす為)
	int drawX = (int)(newX + 0.5f);
	int drawY = (int)(newY + 0.5f);

	switch (type)
	{
	case BlockType::Normal:
		DrawBox(drawX, drawY, drawX + BLOCK_SIZE, drawY + BLOCK_SIZE, Col.GetGra(), TRUE);
        DrawBox(drawX, drawY, drawX + BLOCK_SIZE, drawY + BLOCK_SIZE, Col.GetBla(), FALSE);
		break;
	case BlockType::Gravity:
		DrawBox(drawX, drawY, drawX + BLOCK_SIZE, drawY + BLOCK_SIZE, Col.GetGre(), TRUE);
		break;
	case BlockType::Move:
		DrawBox(drawX, drawY, drawX + BLOCK_SIZE, drawY + BLOCK_SIZE, Col.GetWhi(), TRUE);
		break;
	case BlockType::Goal:
		DrawBox(drawX, drawY, drawX + BLOCK_SIZE, drawY + BLOCK_SIZE, Col.GetYel(), TRUE);
		break;
	case BlockType::Max:
		break;
	case BlockType::NoType:
		break;
	default:
		break;
	}
}