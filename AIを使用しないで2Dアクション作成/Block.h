// ブロッククラス
#pragma once
#include "GameData.h"
#include "Collision.h"
#include <vector>

// ブロックの種類を定義する列挙体
enum class BlockType {
    Normal,     // 通常
    Gravity,    // 重力ブロック
    MoveX,      // 横に動く床
    MoveY,      // 縦に動く床
    Goal,       // ゴールブロック
    Max,        // 定義最大値
    NoType = -1 // 未定義
};

class Block
{
private:
    float x;
    float y;
    float width;
    float height;

    // 重力ブロック用の変数
    float velocityX = 0.0f;
    float velocityY = 0.0f;

    // 動くブロック用の変数
    float originX;
    float originY;
    float moveSpeed = 2.0f;     // 動く速さ
    bool moveForward = true;    // 往復の方向フラグ
    float moveMax;              // 動く量の最大値

    BlockType type;
public:
    Block(float startX, float startY, float w, float h, float move, BlockType t);

    // 更新処理
    bool Update(const std::vector<Block>& blocks, const Rect& playerRect);

    // 描画関数
    void Draw(float centerX, float centerY, float angle) const;

    Rect GetRect()const {
        return { x, y, BLOCK_SIZE, BLOCK_SIZE };
    }

    // プレイヤー側からブロックの種類を確認するためのゲッター
    BlockType GetType() const { return type; }

    // 動くブロックに合わせて移動できるようにする関数
    float GetMoveVelocityX()const {
        if (type == BlockType::MoveX) {
            // trueならプラス、falseならマイナス方向に動く
            return moveForward ? moveSpeed : -moveSpeed;
        }
        if (type == BlockType::Gravity) {
            return velocityX;
        }
        return 0.0f;
    }
    float GetMoveVelocityY()const {
        if (type == BlockType::MoveY) {
            // trueならプラス、falseならマイナス方向に動く
            return moveForward ? moveSpeed : -moveSpeed;
        }
        if (type == BlockType::Gravity) {
            return velocityY;
        }
        return 0.0f;
    }
};