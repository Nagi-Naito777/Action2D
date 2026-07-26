// ゲームプレイシーン
#pragma once
#include "BaseScene.h"
#include "Player.h"
#include "Stage.h"
#include "PlayerController.h"

class PlayScene:public BaseScene
{
private:
	Player player;
	Stage stage;
	PlayerController* controller;

public:
	// コンストラクタ
	PlayScene(SharedData* data);

	// デストラクタ
	~PlayScene();

	void Init() override;
	SceneName Update() override;
	void Draw() const override;
};