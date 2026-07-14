#include "PlayerController.h"

PlayerController::PlayerController(Player *p) {
	targetplayer = p;
}

// 更新処理
void PlayerController::Update() {
	// AとDでキャラクターを操作する設計
	if (CheckHitKey(KEY_INPUT_A)) {
		targetplayer->SetVelocityX(-5.0f);
	}
	else if (CheckHitKey(KEY_INPUT_D)) {
		targetplayer->SetVelocityX(5.0f);
	}
	else {
		targetplayer->SetVelocityX(0.0f);
	}
}