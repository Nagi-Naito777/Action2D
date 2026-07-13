// カプセル化を意識したPlayerクラス
#pragma once
#include "DxLib.h"

class Player {
private:
    // メンバ変数
    float x, y;
    float velocityX, velocityY;
    float speed;

public:
    Player();           // コンストラクタ
    ~Player();          // デストラクタ

    void Update();      // ロジック更新
    void Draw() const;  // 描画

    // ゲッター
    float GetX() const { return x; }
    float GetY() const { return y; }

    // セッター
    void SetPosition(float newX, float newY);

    // プレイヤーコントローラー関数
    void PlayerController();
};