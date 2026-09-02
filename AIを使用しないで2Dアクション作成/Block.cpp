#include "Block.h"
Block::Block(float startX, float startY, float w, float h, float move, BlockType t) {
	x = startX;
	y = startY;
	width = w;
	height = h;
	type = t;

	originX = startX;
	originY = startY;
	moveMax = move;
}

bool Block::Update(const std::vector<Block>& blocks, const Rect& playerRect) {
	switch (type)
	{
	case BlockType::Normal:
		break;
	case BlockType::Gravity:
		// 重力ブロックの重力方向処理
		switch (GravityManager::currentDir)
		{
		case GravityDir::Down:	velocityY += 0.5f; break;
		case GravityDir::Up:	velocityY -= 0.5f; break;
		case GravityDir::Right:	velocityX += 0.5f; break;
		case GravityDir::Left:	velocityX -= 0.5f; break;
		}
		// ブロック同士のX軸当たり判定
		x += velocityX;
		for (const auto& block : blocks) {
			// 自分自身との判定はスキップ
			if (&block == this)continue;

			if (IsHitAABB(GetRect(), block.GetRect())) {
				if (velocityX > 0.0f) {
					x = block.GetRect().Left() - BLOCK_SIZE;
				}
				else if (velocityX < 0.0f) {
					x = block.GetRect().Right();
				}
				velocityX = 0.0f;	// 壁に当たったらリセット
			}
		}
		// プレイヤーとのX軸当たり判定(貫通防止)
		if (IsHitAABB(GetRect(), playerRect)) {
			if (velocityX > 0.0f) { x = playerRect.Left() - BLOCK_SIZE; }
			else if (velocityX < 0.0f) { x = playerRect.Right(); }
			velocityX = 0.0f;
		}

		// ブロック同士のY軸当たり判定
		y += velocityY;
		for (const auto& block : blocks) {
			if (&block == this)continue;

			if (IsHitAABB(GetRect(), block.GetRect())) {
				if (velocityY > 0.0f) {
					y = block.GetRect().Top() - BLOCK_SIZE;
				}
				else if (velocityY < 0.0f) {
					y = block.GetRect().Bottom();
				}
				velocityY = 0.0f;	// 壁に当たったらリセット
			}
		}
		// プレイヤーとのY軸当たり判定(貫通防止)
		if (IsHitAABB(GetRect(), playerRect)) {
			if (velocityY > 0.0f) { y = playerRect.Top() - BLOCK_SIZE; }
			else if (velocityY < 0.0f) { y = playerRect.Bottom(); }
			velocityY = 0.0f;
		}
		break;
	case BlockType::MoveX:
		// 横に動くブロックの移動処理
		if (moveForward) {
			x += moveSpeed;
			// ブロックごとに移動量を変更できるように変数指定
			if (x > originX + moveMax)moveForward = false;
		}
		else {
			x -= moveSpeed;
			if (x < originX)moveForward = true;
		}
		break;
	case BlockType::MoveY:
		// 縦に動くブロックの移動処理
		if (moveForward) {
			y += moveSpeed;
			// ブロックごとに移動量を変更できるように変数指定
			if (y > originY + moveMax)moveForward = false;
		}
		else {
			y -= moveSpeed;
			if (y < originY)moveForward = true;
		}
		break;
	case BlockType::Goal:
		// ゴールとプレイヤーの当たり判定
		if (IsHitAABB(GetRect(), playerRect)) {
			// ゴールに触れた
			return true;
		}
		break;
	case BlockType::Max:
		break;
	case BlockType::NoType:
		break;
	default:
		break;
	}

	// ゴールに触れなかった
	return false;
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
	case BlockType::MoveX:
	case BlockType::MoveY:
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