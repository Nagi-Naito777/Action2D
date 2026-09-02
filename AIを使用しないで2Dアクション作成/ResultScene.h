// リザルト画面クラス
#pragma once
#include "BaseScene.h"

class ResultScene :public BaseScene
{

public:
	// コンストラクタ
	ResultScene(SharedData* data);

	// デストラクタ
	~ResultScene();

	void Init() override;
	SceneName Update() override;
	void Draw() const override;
};

