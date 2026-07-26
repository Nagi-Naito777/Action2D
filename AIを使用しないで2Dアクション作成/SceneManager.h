// シーンを総括するクラス
#pragma once
#include "BaseScene.h"
#include "GameData.h"

class SceneManager {
private:
    BaseScene* currentScene; // 現在実行中のシーン
    SceneName currentName;

    // 共有データの実体
    SharedData sharedData;

public:
    SceneManager();
    ~SceneManager();
    void Update();
    void Draw() const;
    void ChangeScene(SceneName nextName);
};