#include "Player.h"
#include "GameData.h"

Player::Player() {
	x = PLAYER_STARTX;
	y = PLAYER_STARTY;
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
				x = block.GetRect().Left() - PLAYER_SIZE - PLA_BLO_GAP;
			}
			else if (velocityX < 0.0f) {
				x = block.GetRect().Right() + PLA_BLO_GAP;
			}

			velocityX = 0.0f; // 壁にぶつかったら横方向速度をリセット
		}
	}

	// =============================================
	// Y軸方向の移動と当たり判定
	// =============================================
	velocityY += GRAVITY;
	if (velocityY > GRAVITY_MAX) { velocityY = GRAVITY_MAX; }

	y += velocityY;

	// 初期化
	isGrounded = false;

	for (const auto& block : blocks) {
		// 当たり判定
		if (IsHitAABB(GetRect(), block.GetRect())) {
			if (velocityY > 0.0f) {
				y = block.GetRect().Top() - PLAYER_SIZE - PLA_BLO_GAP;
				velocityY = 0.0f;		// 落下を止める
				isGrounded = true;		// 着地フラグをオンにする
			}
			else if (velocityY < 0.0f) {
				y = block.GetRect().Bottom() + PLA_BLO_GAP;
			}

			velocityY = 0.0f; // 壁にぶつかったら縦方向速度をリセット
		}
	}

	// 重力計算
	if (!isGrounded) {
		velocityY += GRAVITY;
	}
}

void Player::Draw(float centerX, float centerY, float angle) const {
	float newX, newY;

	// 自分の座標(x, y)を回転させた先の座標(newX, newY)で計算
	GetRotatedPosition(centerX, centerY, x, y, &newX, &newY, angle);

	DrawBox(newX, newY, newX + PLAYER_SIZE, newY + PLAYER_SIZE, Col.GetRed(), TRUE);
}

