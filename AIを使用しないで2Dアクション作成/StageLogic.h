#pragma once

// ステージ回転アニメーションの挙動フェーズ管理列挙体
enum class RotationPhase {
    None,       // 回転なし
    Shake,      // 回転開始(ガタン！と揺れるイメージ)
    Pause,      // 多少のタメフェーズ
    Accelerate, // 加速しながら回る
    Decelerate  // 減速しながら止まる
};

// 回転方向格納列挙体
enum class RotationType {
    Right90,    // 右に90°
    Left90,     // 左に90°
    Turn180     // 180°回転(方向は右)
};

class StageLogic
{
private:
    RotationPhase currentPhase; // 現在のフェーズ格納
    int timer;                  // タイマー変数
    float currentAngle;         // 現在の角度
    float targetAngle;          // 回転角度の終点
    float startAngle;           // 回転開始角度
    float rotateSpeed;          // 現在の速度

public:
    // コンストラクタ
    StageLogic();

    // 回転開始トリガー関数
    void StartRotation(RotationType type);

    // アニメーション更新処理
    void Update();

    // Stageクラスに角度を教えるための関数
    float GetCurrentAngle()const { return currentAngle; }

    // 今回転中かどうか判断する関数
    bool IsRotating()const { return currentPhase != RotationPhase::None; }
};

