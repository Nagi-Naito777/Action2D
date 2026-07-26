#include "SelectScene.h"

SceneName SelectScene::Update() {
    // 例：1キーを押したらステージ1、2キーを押したらステージ2
    if (CheckHitKey(KEY_INPUT_1) == 1) {
        sharedData->currentStageNo = 1;
        return SceneName::PLAY;
    }
    if (CheckHitKey(KEY_INPUT_2) == 1) {
        sharedData->currentStageNo = 2;
        return SceneName::PLAY;
    }
    if (CheckHitKey(KEY_INPUT_3) == 1) {
        sharedData->currentStageNo = 3;
        return SceneName::PLAY;
    }
    if (CheckHitKey(KEY_INPUT_4) == 1) {
        sharedData->currentStageNo = 4;
        return SceneName::PLAY;
    }
    return SceneName::SELECT;
}

void SelectScene::Draw() const {
    DrawFormatStringToHandle(100, 100, Col.GetWhi(), Font.GetBig(), 
        _T("【ステージ選択】\n1.ステージ1\n2.ステージ2\n3.ステージ3\n4.ステージ4\n5.ステージ5"));
}