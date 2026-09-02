#include "ResultScene.h"
#include "DxLib.h"

// コンストラクタ
ResultScene::ResultScene(SharedData* data) : BaseScene(data) {}

// デストラクタ
ResultScene::~ResultScene(){}

void ResultScene::Init() {

}

SceneName ResultScene::Update() {
	if (CheckHitKey(KEY_INPUT_RETURN)) {
		return SceneName::SELECT;
	}

	return SceneName::RESULT;
}

void ResultScene::Draw() const {
	// クリアメッセージの表示
	DrawString(400, 200, "STAGE CLEAR!", Col.GetYel());

	// 【将来用】合計回転数を表示する想定のコード
	// （SharedData に totalRotations という変数を追加したと仮定）
	// DrawFormatString(400, 300, Col.GetWhi(), "合計回転数 : %d 回", sharedData->totalRotations);

	// 案内文の表示
	DrawString(300, 600, "Press ENTER to Stage Select", Col.GetWhi());
}