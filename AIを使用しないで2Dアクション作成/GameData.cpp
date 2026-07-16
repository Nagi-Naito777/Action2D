#include "GameData.h"
#include "DxLib.h"

void GetRotatedPosition(float centerX, float centerY, float x, float y,
    float* outX, float* outY, float angleDeg) {
    // 角度を度数(Deg)からラジアン(Rad)に変更
    float rad = angleDeg * DX_PI_F / 180.0f;
    float cosA = std::cos(rad);
    float sinA = std::sin(rad);

    // 中心からの相対位置
    float dx = x - centerX;
    float dy = y - centerY;

    // 回転後の相対位置を計算
    float rx = dx * cosA - dy * sinA;
    float ry = dx * sinA + dy * cosA;

    // 中心を足して絶対座標に戻す
    *outX = rx + centerX;
    *outY = ry + centerY;
}