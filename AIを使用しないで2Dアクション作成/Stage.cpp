#include "Stage.h"
#include "Collision.h"[
#include "Block.h"
#include "Player.h"
#include "StageLogic.h"]

Stage::Stage() {
    // ステージ全体サイズ変数
    float stageTotalWidth = STAGE_BLOCK_MAX * BLOCK_SIZE;
    float stageTotalHeight = STAGE_BLOCK_MAX * BLOCK_SIZE;

    // 画面中央に配置するための座標格納
    startX = (int)(WIN_MAX_X - stageTotalWidth) / 2.0f;
    startY = (int)(WIN_MAX_Y - stageTotalHeight) / 2.0f;

    // 周りのブロックを設置するループ文(初期化)
    for (int y = 0; y < STAGE_BLOCK_MAX; y++) {
        for (int x = 0; x < STAGE_BLOCK_MAX; x++) {
            if (y == 0 || y == STAGE_BLOCK_MAX - 1 || x == 0 || x == STAGE_BLOCK_MAX - 1) {
                SetStageBlock(x, y, 0, BlockType::Normal);
            }
        }
    }

    // 中心のX座標
    centerX = WIN_MAX_X / 2.0f;
    // 中心のY座標
    centerY = WIN_MAX_Y / 2.0f;

    stageNo = 3;
}
Stage::~Stage() {}

void Stage::Init(Player &p) {
    // 速度リセット
    p.SetVelocityX(0.0f);
    p.SetVelocityY(0.0f);

    // ステージごとの初期グリッド（マス目）座標用の変数
    int playerGridX = 0;
    int playerGridY = 0;

    // ステージ番号（stageNo）に応じて初期グリッド位置を設定
    switch (stageNo) {
    case 1:
        playerGridX = 10; // 左から10番目のマス
        playerGridY = 16; // 上から16番目のマス
        break;
    case 2:
        playerGridX = 10;
        playerGridY = 10;
        break;
    case 3:
        playerGridX = 2;
        playerGridY = 17;
        break;
    default:
        playerGridX = 10;
        playerGridY = 10;
        break;
    }

    // グリッド座標を、中央寄せを考慮したピクセル座標に変換してセット
    float playerStartX = startX + (playerGridX * BLOCK_SIZE);
    float playerStartY = startY + (playerGridY * BLOCK_SIZE);
    p.SetPosition(playerStartX, playerStartY);

    // ステージの番号ごとに描画する内容を変更する
    for (int y = 0; y < STAGE_BLOCK_MAX; y++) {
        for (int x = 0; x < STAGE_BLOCK_MAX; x++) {
            switch (stageNo) {
            case 1:
                // ゴール配置
                if (x == 10 && y == 1) {
                    SetStageBlock(x, y, 0, BlockType::Goal);
                }
                // 真ん中当たりに一本の線を引くようにブロックを配置
                if ((y == 10 && (x < 18 && x > 2))) {
                    SetStageBlock(x, y, 0, BlockType::Normal);
                }
                break;
            case 2:
                // ゴール配置
                if (x == 10 && y == 1) {
                    SetStageBlock(x, y, 0, BlockType::Goal);
                }
                // 重力ブロック配置
                if (x == 5 && y == 15) {
                    SetStageBlock(x, y, 0, BlockType::Gravity);
                }
                // 通常ブロックを配置
                if ((y == 10 && (x < 18 && x > 2))) {
                    SetStageBlock(x, y, 0, BlockType::Normal);
                }
                break;
            case 3:
                // ゴール配置
                if (x == 10 && y == 1) {
                    SetStageBlock(x, y, 0, BlockType::Goal);
                }
                // 動くブロック配置
                if (x == 10 && y == 15) {
                    SetStageBlock(x, y, 200.0f, BlockType::MoveX);
                }
                if (x == 5 && y == 5) {
                    SetStageBlock(x, y, 200.0f, BlockType::MoveY);
                }
                // 通常ブロックを配置
                if ((y == 10 && (x < 18 && x > 2))) {
                    SetStageBlock(x, y, 0, BlockType::Normal);
                }
            default:
                break;
            }
        }
    }
}

void Stage::Update(Player& player) {
    // 現在のキー状態を取得
    int currentKeyRight = CheckHitKey(KEY_INPUT_RIGHT);
    int currentKeyLeft = CheckHitKey(KEY_INPUT_LEFT);
    int currentKeyUp = CheckHitKey(KEY_INPUT_UP);

    // 「押した瞬間」かどうかを判定
    bool isTriggerRight = (currentKeyRight == 1 && prevKeyRight == 0);
    bool isTriggerLeft = (currentKeyLeft == 1 && prevKeyLeft == 0);
    bool isTriggerUp = (currentKeyUp == 1 && prevKeyUp == 0);

    // 次フレームのために、現在のキー状態を記憶
    prevKeyRight = currentKeyRight;
    prevKeyLeft = currentKeyLeft;
    prevKeyUp = currentKeyUp;

    // もしプレイヤーが地面にいたら回転(地面に接地していたら)
    if (player.IsGrounded()) {
        if (isTriggerRight) {
            TriggerRotation(RotationType::Right90);
        }
        if (isTriggerLeft) {
            TriggerRotation(RotationType::Left90);
        }
        if (isTriggerUp) {
            TriggerRotation(RotationType::Turn180);
        }
    }

    // 更新処理
    logic.Update();

    if (currentState == StageState::Rotating) {
        // 回転が終わったかチェック
        if (!logic.IsRotating()) {
            currentState = StageState::Normal;
        }
        return; // 回転中は以下の処理を行わない
    }

    // --- ここから下は通常時のみ実行 ---
    for (auto& block : blocks) { block.Update(blocks, player.GetRect()); }
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
    if (currentState == StageState::Normal) {
        // 1. ステージ回転の開始
        logic.StartRotation(type);
        currentState = StageState::Rotating;

        // 2. 【追加】回転に応じて重力も切り替える
        // 回転の度合いに合わせて、重力マネージャーを回します
        switch (type) {
        case RotationType::Right90:
            // 時計周りだから右回転したら重力は左なので、3回転して左側に重力を持っていく
            GravityManager::Rotate();
            GravityManager::Rotate();
            GravityManager::Rotate();
            break;
        case RotationType::Left90:
            // 現在地の右側に重力を持っていく
            GravityManager::Rotate();
            
            break;
        case RotationType::Turn180:
            // 2回回して現在地の上側に重力を持っていく
            GravityManager::Rotate();
            GravityManager::Rotate();
            break;
        }
    }
}

// ブロックセット関数
void Stage::SetStageBlock(int x, int y, float move, BlockType bt) {
    blocks.push_back(Block(
        startX + (x * BLOCK_SIZE),
        startY + (y * BLOCK_SIZE),
        BLOCK_SIZE,
        BLOCK_SIZE,
        move,
        bt
    ));
}