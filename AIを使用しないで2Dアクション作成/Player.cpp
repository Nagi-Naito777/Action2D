#include "Player.h"
#include "GameData.h"

Player::Player() {
	x = 0;
	y = 0;
	velocityX = 0;
	velocityY = 0;
	speed = 5.0f;
}

Player::~Player() {}

void Player::Update(const std::vector<Block>&blocks) {
	// 接地判定の初期化
	isGrounded = false;

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
	// X軸方向の移動と当たり判定
	// =============================================
	x += velocityX;

	for (const auto& block : blocks) {
		// 当たり判定
		if (IsHitAABB(GetRect(), block.GetRect())) {
			if (velocityX > 0.0f) {
				x = block.GetRect().Left() - PLAYER_SIZE - PLA_BLO_GAP;

				// 右に向かって落下しているとき、そこが床になる
				if (GravityManager::currentDir == GravityDir::Right)isGrounded = true;
			}
			else if (velocityX < 0.0f) {
				x = block.GetRect().Right() + PLA_BLO_GAP;
				// 左に向かって落下しているとき、そこが床になる
				if (GravityManager::currentDir == GravityDir::Left)isGrounded = true;
			}

			velocityX = 0.0f; // 壁にぶつかったら横方向速度をリセット
		}
	}

	// =============================================
	// Y軸方向の移動と当たり判定
	// =============================================
	y += velocityY;

	for (const auto& block : blocks) {
		// 当たり判定
		if (IsHitAABB(GetRect(), block.GetRect())) {
			if (velocityY > 0.0f) {
				y = block.GetRect().Top() - PLAYER_SIZE - PLA_BLO_GAP;
				// 下に向かって落下しているとき、そこが床になる
				if (GravityManager::currentDir == GravityDir::Down)isGrounded = true;
			}
			else if (velocityY < 0.0f) {
				y = block.GetRect().Bottom() + PLA_BLO_GAP;
				// 上に向かって落下しているとき、そこが床になる
				if (GravityManager::currentDir == GravityDir::Up)isGrounded = true;
			}

			velocityY = 0.0f; // 壁にぶつかったら縦方向速度をリセット
		}
	}
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

