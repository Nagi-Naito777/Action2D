// ステージクラス
#pragma once
#include <vector>
#include "Block.h"
#include "Player.h"

class Stage {
private:
    std::vector<Block> blocks;

public:
    Stage();
    ~Stage();

    // ステージの初期化（ここでブロックを配置する）
    void Init();

    // 更新処理（操作中のプレイヤーを受け取り、ブロックとの当たり判定を行う）
    void Update(Player& player);

    // 描画処理（所属するすべてのブロックを描画する）
    void Draw() const;
};