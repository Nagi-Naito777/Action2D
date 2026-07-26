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
	// ステージとプレイヤーの初期化
	stage.Init(player,sharedData->currentStageNo);
}

SceneName PlayScene::Update() {
	// 移動処理
	controller->Update();

	// ステージ処理
	stage.Update(player);

	// ESCキーでタイトルに戻る（デバッグ用にあると便利です）
	if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) {
		return SceneName::TITLE;
	}

	return SceneName::PLAY;
}

void PlayScene::Draw() const {

	stage.Draw(player);
}