// タイトルシーン管理クラス
#pragma once
#include "BaseScene.h"

class TitleScene :public BaseScene
{
public:
	void Init() override {};

	SceneName Update() override;

	void Draw() const override;
};