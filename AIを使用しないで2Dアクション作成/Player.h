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
    // 床から受ける移動量変数
    float carryVX, carryVY;

    bool isGrounded;    // 地面についているか判定
public:
    Player();           // コンストラクタ
    ~Player();          // デストラクタ

    void Update(const std::vector<Block>& blocks);      // ロジック更新
    void Draw(float centerX, float centerY, float angle) const;  // 描画

    // ゲッター
    float GetX() const { return x; }
    float GetY() const { return y; }
    Rect GetRect() const {
        return { x, y, PLAYER_SIZE, PLAYER_SIZE };
    }

    // X軸移動の判定用
    Rect GetRectX() const {
        float margin = 2.0f; // 2ピクセル浮かせる
        return { x, y + margin, PLAYER_SIZE, PLAYER_SIZE - (margin * 2.0f) };
    }

    // Y軸移動の判定用
    Rect GetRectY() const {
        float margin = 2.0f;
        return { x + margin, y, PLAYER_SIZE - (margin * 2.0f), PLAYER_SIZE };
    }
    // ※X軸とY軸を少し浮かせることで動く床の挙動バグを無くす

    // 地面に着地しているか判定取得
    bool IsGrounded()const { return isGrounded; }

    // セッター
    void SetPosition(float setX, float setY) {
        x = setX;
        y = setY;
    }
    void SetVelocityX(float setX) {
        velocityX = setX;
    }
    void SetVelocityY(float setY) {
        velocityY = setY;
    }
};