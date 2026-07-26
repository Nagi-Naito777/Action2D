#pragma once
#include "BaseScene.h"

class SelectScene : public BaseScene {
public:
    SelectScene(SharedData* data) : BaseScene(data) {}

    SceneName Update() override;

    void Draw() const override;
};