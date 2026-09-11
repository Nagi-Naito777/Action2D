#include "Player.h"
#include "GameData.h"

Player::Player() {
	x = 0;
	y = 0;
	velocityX = 0;
	velocityY = 0;
	speed = 5.0f;
	carryVX = 0.0f;
	carryVY = 0.0f;
}

Player::~Player() {}

// プレイヤーの当たり判定処理関数
void Player::MoveAndCheckCollision(const std::vector<Block>& blocks, bool isXAxis) {
	// X軸かY軸かで、操作する変数への「参照」を切り替える
	float& pos = isXAxis ? x : y;
	float& vel = isXAxis ? velocityX : velocityY;

	// 重力方向の条件切り替え
	GravityDir posGravity = isXAxis ? GravityDir::Right : GravityDir::Down;
	GravityDir negGravity = isXAxis ? GravityDir::Left : GravityDir::Up;

	// 移動させる計算
	pos += vel;

	for (const auto& block : blocks) {
		// ゴール、スイッチ、無効状態のブロックは当たり判定処理をスキップ(すり抜けるようにする)
		if (block.GetType() == BlockType::Goal ||
			block.GetType() == BlockType::Switch ||
			!block.IsActive()) {
			continue;
		}

		// 毎回のループで最新状態のRectを取得
		Rect playerRect = isXAxis ? GetRectX() : GetRectY();

		// 当たり判定処理
		if (IsHitAABB(playerRect, block.GetRect())) {
			if (vel > 0.0f) {
				// 右 または 下に向かって落下しているとき、そこが床になる
				pos = isXAxis ? (block.GetRect().Left() - PLAYER_SIZE - PLA_BLO_GAP)
					: (block.GetRect().Top() - PLAYER_SIZE - PLA_BLO_GAP);

				if (GravityManager::currentDir == posGravity) {
					isGrounded = true;
					// 直接座標を足さず、次回の移動用に保存
					carryVX = block.GetMoveVelocityX();
					carryVY = block.GetMoveVelocityY();
				}
			}
			else if (vel < 0.0f) {
				// 左 または 上に向かって落下しているとき、そこが床になる
				pos = isXAxis ? (block.GetRect().Right() + PLA_BLO_GAP)
					: (block.GetRect().Bottom() + PLA_BLO_GAP);

				if (GravityManager::currentDir == negGravity) {
					isGrounded = true;
					carryVX = block.GetMoveVelocityX();
					carryVY = block.GetMoveVelocityY();
				}
			}

			// 壁にぶつかったら速度をリセット
			vel = 0.0f;
		}
	}
}

void Player::Update(const std::vector<Block>&blocks) {
	// 接地判定の初期化
	isGrounded = false;

	// 動く床分の座標をずらす
	x += carryVX;
	y += carryVY;

	// リセット
	carryVX = 0.0f;
	carryVY = 0.0f;

	// 現在の重力方向に応じて速度を加算、制御する
	switch (GravityManager::currentDir)
	{
	case GravityDir::Down:
		velocityY += GRAVITY;
		if (velocityY > GRAVITY_MAX)velocityY = GRAVITY_MAX;
		break;
	case GravityDir::Up:
		velocityY -= GRAVITY;
		if (velocityY < -GRAVITY_MAX)velocityY = -GRAVITY_MAX;
		break;
	case GravityDir::Right:
		velocityX += GRAVITY;
		if (velocityX > GRAVITY_MAX)velocityX = GRAVITY_MAX;
		break;
	case GravityDir::Left:
		velocityX -= GRAVITY;
		if (velocityX < -GRAVITY_MAX)velocityX = -GRAVITY_MAX;
		break;
	default:
		break;
	}

	// =============================================
	// 移動と当たり判定関数
	// =============================================
	MoveAndCheckCollision(blocks, true);  // X軸の移動と判定
	MoveAndCheckCollision(blocks, false); // Y軸の移動と判定
}

void Player::Draw(float centerX, float centerY, float angle) const {
	// プレイヤーの中心座標を計算する
	float playerCenterX = x + (PLAYER_SIZE / 2.0f);
	float playerCenterY = y + (PLAYER_SIZE / 2.0f);

	float rotatedCenterX, rotatedCenterY;

	// 中心を基準に回転させる
	GetRotatedPosition(centerX, centerY, playerCenterX, playerCenterY, &rotatedCenterX, &rotatedCenterY, angle);

	// 回転後の中心座標から、描画関数に渡すための左上座標を逆算
	float newX = rotatedCenterX - (PLAYER_SIZE / 2.0f);
	float newY = rotatedCenterY - (PLAYER_SIZE / 2.0f);

	// 座標を整数に変換してから描画する
	int drawX = (int)(newX + 0.5f);
	int drawY = (int)(newY + 0.5f);

	DrawBox(drawX, drawY, drawX + PLAYER_SIZE, drawY + PLAYER_SIZE, Col.GetRed(), TRUE);
}

