#include "PlayScene.h"
#include "DxLib.h"

// コンストラクタで親クラスにデータを渡す
PlayScene::PlayScene(SharedData* data) : BaseScene(data) {
	controller = new PlayerController(&player);
}

PlayScene::~PlayScene() {
	delete controller;
}

void PlayScene::Init() {
	// ステージ開始時の必須リセット
	sharedData->isClear = false;

	// ステージとプレイヤーの初期化
	stage.Init(player,sharedData->currentStageNo);
}

SceneName PlayScene::Update() {
	Sound.Play("BGM_PLAY", true, false);

	// 移動処理
	controller->Update();

	// ステージ処理
	bool isGoalReached = stage.Update(player);

	// もしゴールしたらリザルトシーンへ移行
	if (isGoalReached) {
		// 共有データにクリアしたという判定を渡す
		sharedData->isClear = true;
		Sound.Play("SE_CLEAR", false, true);
		Sound.Stop("BGM_PLAY");
		Sound.Play("BGM_MAIN", true, true);
		return SceneName::RESULT;
	}

	// ESCキーでタイトルに戻る
	if (CheckHitKey(KEY_INPUT_ESCAPE)) {
		Sound.Play("SE_SYSTEM", false, true);
		Sound.Stop("BGM_PLAY");
		Sound.Play("BGM_MAIN", true, true);
		return SceneName::TITLE;
	}

	// スペースキーでステージリセット
	if (CheckHitKey(KEY_INPUT_SPACE)) {
		Sound.Play("SE_SYSTEM", false, true);
		Init();
	}

	return SceneName::PLAY;
}

void PlayScene::Draw() const {

	stage.Draw(player);

}