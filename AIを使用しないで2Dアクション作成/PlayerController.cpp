#include "PlayerController.h"

PlayerController::PlayerController(Player *p) {
	targetplayer = p;
}

// 更新処理
void PlayerController::Update() {
	// 操作による移動を0にする
	float moveSpeedX = 0.0f;
	float moveSpeedY = 0.0f;


	// プレイヤーが地面にいるときだけキー入力を受け付ける
	if (targetplayer->IsGrounded()) {
		// AとDでキャラクターを操作する設計
		if (CheckHitKey(KEY_INPUT_A)) {
			switch (GravityManager::currentDir) {
			case GravityDir::Down:  moveSpeedX = -5.0f; break; // 重力下：Aで左へ
			case GravityDir::Up:    moveSpeedX = +5.0f; break; // 重力上：Aで右へ（画面が逆さまなため）
			case GravityDir::Right: moveSpeedY = +5.0f; break; // 重力右：Aで上へ（画面が左に倒れているため）
			case GravityDir::Left:  moveSpeedY = -5.0f; break; // 重力左：Aで下へ（画面が右に倒れているため）
			}
		}
		else if (CheckHitKey(KEY_INPUT_D)) {
			switch (GravityManager::currentDir) {
			case GravityDir::Down:  moveSpeedX = +5.0f; break; // 重力下：Dで右へ
			case GravityDir::Up:    moveSpeedX = -5.0f; break; // 重力上：Dで左へ
			case GravityDir::Right: moveSpeedY = -5.0f; break; // 重力右：Dで下へ
			case GravityDir::Left:  moveSpeedY = +5.0f; break; // 重力左：Dで上へ
			}
		}
	}
	// 決定した移動速度をセット
	if (GravityManager::currentDir == GravityDir::Down || GravityManager::currentDir == GravityDir::Up) {
		targetplayer->SetVelocityX(moveSpeedX);
	}
	else if (GravityManager::currentDir == GravityDir::Left || GravityManager::currentDir == GravityDir::Right) {
		targetplayer->SetVelocityY(moveSpeedY);
	}
}