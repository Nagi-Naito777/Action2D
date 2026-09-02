#include "SceneManager.h"
#include "TitleScene.h"
#include "SelectScene.h"
#include "PlayScene.h"
#include "ResultScene.h"

SceneManager::SceneManager() {
	currentScene = new TitleScene(&sharedData);
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
	case SceneName::TITLE:  currentScene = new TitleScene(&sharedData);  break;
	case SceneName::SELECT: currentScene = new SelectScene(&sharedData); break;
	case SceneName::PLAY:   currentScene = new PlayScene(&sharedData);   break;
	case SceneName::RESULT: currentScene = new ResultScene(&sharedData); break;
		break;
	}

	currentScene->Init();
	currentName = nextName;
}