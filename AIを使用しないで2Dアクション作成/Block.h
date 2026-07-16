// ブロッククラス
#pragma once
#include "GameData.h"
#include "Collision.h"

// ブロックの種類を定義する列挙体
enum class BlockType {
    Normal,     // 通常
    Gravity,    // 重力ブロック
    Move,       // 動くブロック
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

    BlockType type;
public:
    Block(float x, float y, float w, float h, BlockType t);

    // 更新処理
    void Update();

    // 描画関数
    void Draw(float centerX, float centerY, float angle) const;

    Rect GetRect()const {
        return { x, y, BLOCK_SIZE, BLOCK_SIZE };
    }
};