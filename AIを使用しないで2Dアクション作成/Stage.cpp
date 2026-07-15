#include "Stage.h"
#include "Collision.h"

Stage::Stage() {}
Stage::~Stage() {}

void Stage::Init() {
    // 周りのブロックを設置するループ文
    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 20; x++) {
            if (y == 0 || y == 19 || x == 0 || x == 19) {
                blocks.push_back(Block(
                    x * BLOCK_SIZE,
                    y * BLOCK_SIZE,
                    x + BLOCK_SIZE,
                    y + BLOCK_SIZE
                ));
            }
        }
    }
}

void Stage::Update(Player& player) {
    // 各ブロックの更新処理（動く床などがあればここで動く）
    for (auto& block : blocks) {
        block.Update();
    }

    // プレイヤーの移動とブロックとの当たり判定を解決
    // プレイヤーにステージ内のブロックのリストを渡して、内部で解決させる（パターンA）
    player.Update(blocks);
}

void Stage::Draw() const {
    // ステージに配置されているすべてのブロックを描画する
    for (const auto& block : blocks) {
        block.Draw();
    }
}