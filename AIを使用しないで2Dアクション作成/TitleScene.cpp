#include "TitleScene.h"

SceneName TitleScene::Update() {
	// エンターキーを押したらステージ選択画面に移行
	if (CheckHitKey(KEY_INPUT_RETURN)) {
		return SceneName::SELECT;
	}
	return SceneName::TITLE;
}

void TitleScene::Draw() const {
	DrawFormatStringToHandle(100, 100, Col.GetWhi(), Font.GetBig(), _T("ミステリアス\n  リンネキューブ"));
}