#pragma once
#include "BaseScene.h"
#include "Stage.h"
// ===================================================
// 元々デバッグ用のクラスとして作成したが、
// ステージ作成機能を実装したら面白いのではと思い
// そのままステージ作成モードとして実装しました
// ===================================================

#include "GameData.h"
#include "PlayerController.h"
#include <string>

class DebugScene : public BaseScene {
private:
	// ステージサイズ配列
	char stageMap[STAGE_BLOCK_MAX][STAGE_BLOCK_MAX];

	// プレイモード切り替えフラグ
	bool isPlayMode;
	// 選択されたステージの番号
	int currentStageNo;
	// 現在選択中の配置ブロック文字
	char selectedBlockChar;

	Stage stage;
	Player player;
	PlayerController* controller;

	char prevKeys[256];
	int mouseClickPrev;

	void SaveDebugStage(const std::string& filename)const;
	void LoadDebugStage(const std::string& filename);

public:
	DebugScene(SharedData* data);
	~DebugScene();

	void Init()override;
	SceneName Update()override;
	void Draw()const override;

};