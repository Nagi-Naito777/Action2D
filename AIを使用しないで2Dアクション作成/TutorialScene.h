#pragma once
#include "BaseScene.h"
#include "Player.h"
#include "Stage.h"
#include "PlayerController.h"
#include "Tutorial.h"
#include <vector>

class TutorialScene :public BaseScene
{
private:
	Player player;
	Stage stage;
	PlayerController* controller;

	int currentStep = 1;                   // 現在のチュートリアル段階
	const int MAX_TUTORIAL_STEPS = 3;      // チュートリアルの全ステップ数

	int currentPhase;                      // 1ステップ内の現在のフェーズ(手順)
	std::vector<TutorialTask> tasks;       // 現在のステップの手順リスト

	int waitTimer = 0;                     // 【追加】フェーズ内の待機用タイマー

	void LoadTasks(int step);              // ステップに応じた手順をロード
	void ApplyAllowedActions();            // 許可アクションを各クラスに適用する

public:
	TutorialScene(SharedData* data);
	~TutorialScene();

	void Init() override;
	SceneName Update() override;
	void Draw() const override;
};