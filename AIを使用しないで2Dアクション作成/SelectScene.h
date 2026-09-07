#pragma once
#include "BaseScene.h"

class SelectScene : public BaseScene {
public:
    SelectScene(SharedData* data) : BaseScene(data) {}

    SceneName Update() override;

    void Draw() const override;

private:
    // ステージ選択カーソル構造体
    struct Cursol {
        int X = 0;
        int Y = 0;
        const int X_Lim = 3;    // X座標の限界地
        const int Y_Lim = 2;    // Y座標の限界地
    };
    Cursol Cur;
};