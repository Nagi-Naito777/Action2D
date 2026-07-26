// シーン管理クラスの大元ヘッダー
#pragma once
#include "GameData.h"

class BaseScene
{
protected:
	// すべてのシーンからアクセスできる共有データへのポインタ
	SharedData* sharedData;

public:
	// コンストラクタ
	BaseScene(SharedData* data) :sharedData(data) {}

	// デストラクタ
	virtual ~BaseScene() {}

	// 初期化処理
	virtual void Init(){}

	// 更新処理
	virtual SceneName Update() = 0;

	// 描画処理
	virtual void Draw()const = 0;
};

