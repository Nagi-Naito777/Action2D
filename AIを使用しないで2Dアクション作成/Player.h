// カプセル化を意識したPlayerクラス
#pragma once
#include "DxLib.h"
#include "GameData.h"
#include "Collision.h"
#include "Block.h"
#include <vector>

class Player {
private:
    // メンバ変数
    float x, y;
    float velocityX, velocityY;
    float speed;

    bool isGrounded;    // 地面についているか判定
public:
    Player();           // コンストラクタ
    ~Player();          // デストラクタ

    void Update(const std::vector<Block>& blocks);      // ロジック更新
    void Draw() const;  // 描画

    // ゲッター
    float GetX() const { return x; }
    float GetY() const { return y; }
    Rect GetRect() const {
        return { x, y, PLAYER_SIZE, PLAYER_SIZE };
    }

    // セッター
    void SetPosition(float newX, float newY);
    void SetVelocityX(float setX) {
        velocityX = setX;
    }
};