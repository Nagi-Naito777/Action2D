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
    
public:
    Stage();
    ~Stage();

    // ステージの初期化（ここでブロックを配置する）
    void Init();

    // 更新処理（操作中のプレイヤーを受け取り、ブロックとの当たり判定を行う）
    void Update(Player& player);

    // 描画処理（所属するすべてのブロックを描画する）
    // ※プレイヤー情報も同時に格納して回転ロジックのアニメーション描画をやりやすくする
    void Draw(const Player& player) const;

    // テスト用の回転トリガー関数を追加
    void TriggerRotation(RotationType type);
};