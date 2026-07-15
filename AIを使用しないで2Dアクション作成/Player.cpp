#include "Player.h"
#include "GameData.h"

Player::Player() {
	x = 100.0f;
	y = 100.0f;
	velocityX = 0;
	velocityY = 0;
	speed = 5.0f;
}

Player::~Player() {}

void Player::Update(const std::vector<Block>&blocks) {
	// =============================================
	// X軸方向の移動と当たり判定
	// =============================================
	x += velocityX;

	for (const auto& block : blocks) {
		// 当たり判定
		if (IsHitAABB(GetRect(), block.GetRect())) {
			if (velocityX > 0.0f) {
				x = block.GetRect().Left() - PLAYER_SIZE;
			}
			else if (velocityX < 0.0f) {
				x = block.GetRect().Right();
			}

			velocityX = 0.0f; // 壁にぶつかったら横方向速度をリセット
		}
	}

	// =============================================
	// Y軸方向の移動と当たり判定
	// =============================================
	velocityY += GRAVITY;
	if (velocityY > GRAVITY_MAX) { velocityY = GRAVITY_MAX; }

	// 初期化
	isGrounded = false;

	for (const auto& block : blocks) {
		// 当たり判定
		if (IsHitAABB(GetRect(), block.GetRect())) {
			if (velocityY > 0.0f) {
				y = block.GetRect().Top() - PLAYER_SIZE;
				velocityY = 0.0f;		// 落下を止める
				isGrounded = true;		// 着地フラグをオンにする
			}
			else if (velocityY < 0.0f) {
				y = block.GetRect().Bottom();
			}

			velocityY = 0.0f; // 壁にぶつかったら縦方向速度をリセット
		}
	}

	// 重力計算
	if (!isGrounded) {
		velocityY += GRAVITY;
	}
}

void Player::Draw() const{
	DrawBox(x, y, x + PLAYER_SIZE, y + PLAYER_SIZE, Col.GetRed(), TRUE);
}

