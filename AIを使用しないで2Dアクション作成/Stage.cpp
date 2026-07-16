#include "Stage.h"
#include "Collision.h"

Stage::Stage() {
    // 周りのブロックを設置するループ文(初期化)
    for (int y = 0; y < STAGE_BLOCK_MAX; y++) {
        for (int x = 0; x < STAGE_BLOCK_MAX; x++) {
            if (y == 0 || y == STAGE_BLOCK_MAX - 1 || x == 0 || x == STAGE_BLOCK_MAX - 1) {
                blocks.push_back(Block(
                    STAGE_POSX + (x * BLOCK_SIZE),
                    STAGE_POSY + (y * BLOCK_SIZE),
                    x + BLOCK_SIZE,
                    y + BLOCK_SIZE,
                    BlockType::Normal
                ));
            }
        }
    }

    // 中心のX座標 = 左上のX + (ブロックの数 * ブロックのサイズ) / 2
    centerX = STAGE_POSX + (STAGE_BLOCK_MAX * BLOCK_SIZE) / 2.0f;

    // 中心のY座標 = 左上のY + (ブロックの数 * ブロックのサイズ) / 2
    centerY = STAGE_POSY + (STAGE_BLOCK_MAX * BLOCK_SIZE) / 2.0f;

    stageNo = 1;
}
Stage::~Stage() {}

void Stage::Init() {
    // ステージの番号ごとに描画する内容を変更する
    for (int y = 0; y < STAGE_BLOCK_MAX; y++) {
        for (int x = 0; x < STAGE_BLOCK_MAX; x++) {
            switch (stageNo) {
            case 1:
                if (x == 10 && y == 1) {
                    blocks.push_back(Block(
                        STAGE_POSX + (x * BLOCK_SIZE),
                        STAGE_POSY + (y * BLOCK_SIZE),
                        x + BLOCK_SIZE,
                        y + BLOCK_SIZE,
                        BlockType::Goal
                    ));
                }
            case 2:
            case 3:
            case 4:
            default:
                break;
            }
        }
    }
}

void Stage::Update(Player& player) {
    // ==========================================
    // 【テスト用】キー入力で回転を発動させる
    // ==========================================
    if (CheckHitKey(KEY_INPUT_RIGHT) == 1) {
        TriggerRotation(RotationType::Right90);
    }
    if (CheckHitKey(KEY_INPUT_LEFT) == 1) {
        TriggerRotation(RotationType::Left90);
    }
    if (CheckHitKey(KEY_INPUT_UP) == 1) {
        TriggerRotation(RotationType::Turn180);
    }

    // ==========================================
    // 本来の更新処理
    // ==========================================
    logic.Update();

    if (currentState == StageState::Rotating) {
        // 回転が完全に終わったか（PhaseがNoneに戻ったか）をチェック
        if (!logic.IsRotating()) {
            currentState = StageState::Normal; // 通常状態に戻す
        }
        return; // 回転中は以下の処理（重力や当たり判定）を行わない
    }

    // --- ここから下は通常時のみ実行 ---
    for (auto& block : blocks) { block.Update(); }
    player.Update(blocks);
}

void Stage::Draw(const Player& player) const {
    // 現在の座標の角度を取得
    float currentAngle = logic.GetCurrentAngle();

    // ステージに配置されているすべてのブロックを描画する
    for (const auto& block : blocks) {
        block.Draw(centerX, centerY, currentAngle);
    }

    // プレイヤーの描画
    player.Draw(centerX, centerY, currentAngle);

    // 文字を描画（X:10, Y:10 の位置に、白い文字で表示）
    DrawFormatStringToHandle(10, 10, Col.GetWhi(), Font.GetNormal(), _T("現在の角度: %.2f"), currentAngle);

    // 状態がわかりやすいように、回転中かどうかを表示
    if (currentState == StageState::Rotating) {
        DrawFormatStringToHandle(10, 50, Col.GetYel(), Font.GetNormal(), _T("状態: 回転中 (操作ロック)"));
    }
    else {
        DrawFormatStringToHandle(10, 50, Col.GetGre(), Font.GetNormal(), _T("状態: 通常プレイ (操作可能)"));
    }
}

void Stage::TriggerRotation(RotationType type) {
    // もし通常状態（操作可能）なら、回転を開始して状態を「回転中」に切り替える
    if (currentState == StageState::Normal) {
        logic.StartRotation(type);
        currentState = StageState::Rotating;
    }
}