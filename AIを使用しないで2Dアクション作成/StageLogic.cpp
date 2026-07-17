#include "StageLogic.h"
#include <cmath>

// コンストラクタ
StageLogic::StageLogic() {
	currentPhase = RotationPhase::None;
	timer = 0;
	currentAngle = 0.0f;
	targetAngle = 0.0f;
	startAngle = 0.0f;
	rotateSpeed = 0.0f;
}

// 回転開始トリガー関数
void StageLogic::StartRotation(RotationType type) {
	if (currentPhase != RotationPhase::None)return; // 回転中なら処理しない

	startAngle = currentAngle;	// 今の角度をスタート地点として記録
	rotateSpeed = 0.0;

	// 指定されたパターン合わせて目標角度を設定
	switch (type)
	{
	case RotationType::Right90:
		targetAngle = startAngle + 90.0f;
		break;
	case RotationType::Left90:
		targetAngle = startAngle - 90.0f;
		break;
	case RotationType::Turn180:
		targetAngle = startAngle + 180.0f;
		break;
	default:
		break;
	}
	
	currentPhase = RotationPhase::Shake;
	timer = 0; // タイマーリセット
}

// アニメーション更新処理(フェーズごとにそれぞれ別々の処理をしてもらう)
void StageLogic::Update() {
	if (currentPhase == RotationPhase::None) return;

	timer++;

	// 目標角度の大きさ比較(スタートより大きいか小さいかを比較する三項演算子)
	float dir = (targetAngle > startAngle) ? 1.0f : -1.0f;

	// 現在どれぐらい回ったか
	float movedAmount = std::abs(currentAngle - startAngle);
	// 全体でどれぐらい回す予定か(90 or 180)
	float totalAmount = std::abs(targetAngle - startAngle);

	switch (currentPhase)
	{
	case RotationPhase::Shake:
		// timerの経過フレーム数に応じて、手動で角度をズラす
		if (timer < 5) {
			currentAngle = startAngle - (3.0f * dir); // 逆方向に3度ズレる
		}
		else if (timer < 10) {
			currentAngle = startAngle + (1.5f * dir); // 反動で少し進む
		}
		else if (timer < 15) {
			currentAngle = startAngle - (0.5f * dir); // もう一度小さく戻る
		}
		else {
			currentAngle = startAngle; // ピタッと止まる
		}

		if (timer > 20) {
			currentPhase = RotationPhase::Pause;
			timer = 0; // 毎回必ずリセット
		}
		break;
	case RotationPhase::Pause:
		if (timer > 25) {
			currentPhase = RotationPhase::Accelerate;
			timer = 0;
		}
		break;
	case RotationPhase::Accelerate:
		rotateSpeed += 0.3f * dir;
		currentAngle += rotateSpeed;

		// 目標の「半分」まで回ったら減速フェーズへ移行
		if (movedAmount >= totalAmount / 2.0f) {
			currentPhase = RotationPhase::Decelerate;
		}
		break;
	case RotationPhase::Decelerate:
		rotateSpeed -= 0.3f * dir;
		currentAngle += rotateSpeed;

		// 回転しすぎを防ぐための処理
		if (std::abs(rotateSpeed) <= 0.3f || movedAmount >= totalAmount) {
			currentAngle = targetAngle;	// ズレの補正
			currentPhase = RotationPhase::None;
			timer = 0;

			// 角度補正用ループ
			while (currentAngle >= 360.0f) {
				// 360度以上になった場合の補正
				currentAngle -= 360.0f;
			}
			while (currentAngle < 0.0f) {
				// 0度以下になった場合の補正
				currentAngle += 360.0f;
			}
		}
		break;
	default:
		break;
	}
}