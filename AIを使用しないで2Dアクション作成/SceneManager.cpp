#include "SceneManager.h"
#include "TitleScene.h"

SceneManager::SceneManager() {
	currentScene = new TitleScene();
	currentScene->Init();
	currentName = SceneName::TITLE;
}

SceneManager::~SceneManager() {
	delete currentScene;
}

void SceneManager::Update() {
	// 現在のシーンを更新し、戻り値に次のシーンを受け取る
	SceneName nextScene = currentScene->Update();

	// 戻り値が今のシーンと違っていたら切り替え処理
	if (nextScene != currentName) {
		ChangeScene(nextScene);
	}
}

void SceneManager::Draw()const {
	// 現在シーンの描画
	currentScene->Draw();
}

void SceneManager::ChangeScene(SceneName nextName) {
	// 古いシーンを削除
	delete currentScene;

	// 新しいシーンを生成
	switch (nextName) {
	case SceneName::TITLE: currentScene = new TitleScene(); break;
	case SceneName::SELECT:
	case SceneName::PLAY:
	case SceneName::RESULT:
		break;
	}

	currentScene->Init();
	currentName = nextName;
}