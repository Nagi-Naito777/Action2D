// 当たり判定計算ヘッダー
#pragma once
struct Rect {
    float x;
    float y;
    float width;
    float height;

    float Left() const { return x; }
    float Right() const { return x + width; }
    float Top() const { return y; }
    float Bottom() const { return y + height; }
};

// 矩形同士の当たり判定関数
bool IsHitAABB(const Rect& rectA, const Rect& rectB);