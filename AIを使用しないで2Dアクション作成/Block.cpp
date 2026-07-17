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
	// ブロック自身の中心座標を計算
	float blockCenterX = x + (BLOCK_SIZE / 2.0f);
	float blockCenterY = y + (BLOCK_SIZE / 2.0f);

	float rotatedCenterX, rotatedCenterY;

	// ブロックの中心をステージの中心を軸にして回転させる
	GetRotatedPosition(centerX, centerY, blockCenterX, blockCenterY, &rotatedCenterX, &rotatedCenterY, angle);

	// 回転後の中心座標から描画関数に渡すための左上座標の逆算
	float newX = rotatedCenterX - (BLOCK_SIZE / 2.0f);
	float newY = rotatedCenterY - (BLOCK_SIZE / 2.0f);

	// 座標を変数に変換
	int drawX = (int)(newX + 0.5f);
	int drawY = (int)(newY + 0.5f);

	switch (type)
	{
	case BlockType::Normal:
		DrawBox(drawX, drawY, drawX + BLOCK_SIZE, drawY + BLOCK_SIZE, Col.GetGra(), TRUE);
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