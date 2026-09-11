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

void Block::MoveAndCheckCollision(const std::vector<Block>& blocks, bool isXAxis) {
	// X軸かY軸かで、操作する変数への「参照」を切り替える
	float& pos = isXAxis ? x : y;
	float& vel = isXAxis ? velocityX : velocityY;

	// 移動量を加算
	pos += vel;

	for (const auto& block : blocks) {
		// 自分自身との判定はスキップ
		if (&block == this) {
			continue;
		}
		// ゴール、スイッチ、他の重力ブロック、無効状態のブロックはすり抜ける
		if (block.GetType() == BlockType::Goal ||
			block.GetType() == BlockType::Switch ||
			//block.GetType() == BlockType::Gravity ||
			!block.IsActive()) {
			continue;
		}

		// 当たり判定
		if (IsHitAABB(GetRect(), block.GetRect())) {
			if (vel > 0.0f) {
				// プラス方向(右 または 下)
				pos = isXAxis ? (block.GetRect().Left() - BLOCK_SIZE)
					: (block.GetRect().Top() - BLOCK_SIZE);
			}
			else if (vel < 0.0f) {
				// マイナス方向(左 または 上)
				pos = isXAxis ? block.GetRect().Right()
					: block.GetRect().Bottom();
			}

			// 壁に当たったら速度をリセット
			vel = 0.0f;
		}
	}
}

bool Block::Update(const std::vector<Block>& blocks, const Rect& playerRect) {
	// 無効状態(押されていない時のゴール等)なら、更新・ゴール判定を行わずに終わる
	if (!IsActive()) {
		return false;
	}
	
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

		// 移動と当たり判定関数
		MoveAndCheckCollision(blocks, true);  // X軸の移動と判定
		MoveAndCheckCollision(blocks, false); // Y軸の移動と判定

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
	// 無効状態(スイッチが押されていない時)なら描画しない
	if (!IsActive()) {
		return;
	}
	
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
	case BlockType::Switch:
		// スイッチブロックのみ少し透けるようにする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		DrawBox(drawX, drawY, drawX + BLOCK_SIZE, drawY + BLOCK_SIZE, Col.GetSky(), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
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