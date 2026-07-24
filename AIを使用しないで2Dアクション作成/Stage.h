// ステージクラス
#pragma once
#include <vector>
#include "Block.h"
#include "Player.h"
#include "StageLogic.h"

enum class StageState {
    Normal,     // 通常状態(操作可能・当たり判定有)
    Rotating    // 回転状態(操作不可・当たり判定無)
};

class Stage {
private:
    std::vector<Block> blocks;
    StageState currentState;    // 今のステージの状態格納
    StageLogic logic;           // ステージ回転ロジック格納

    // ステージ数格納変数
    int stageNo;

    // ステージの中心座標
    float centerX;
    float centerY;

    // 画面中央に配置するための座標格納
    int startX;
    int startY;

    // 前回のキー入力状態を記憶する変数
    int prevKeyRight = 0;
    int prevKeyLeft = 0;
    int prevKeyUp = 0;
    
public:
    Stage();
    ~Stage();

    // ステージの初期化
    void Init(Player& p, int stage);

    // 更新処理
    void Update(Player& player);

    // 描画処理
    void Draw(const Player& player) const;

    // 回転トリガー関数
    void TriggerRotation(RotationType type);

    // ブロックセット関数
    void SetStageBlock(int x, int y, float move, BlockType bt);
};