#include "Stage.h"
#include "Collision.h"
#include "Block.h"
#include "Player.h"
#include "StageLogic.h"
#include <fstream>
#include <string>

Stage::Stage() {
    // ステージ全体サイズ変数
    float stageTotalWidth = STAGE_BLOCK_MAX * BLOCK_SIZE;
    float stageTotalHeight = STAGE_BLOCK_MAX * BLOCK_SIZE;

    // 画面中央に配置するための座標格納
    startX = (int)(WIN_MAX_X - stageTotalWidth) / 2.0f;
    startY = (int)(WIN_MAX_Y - stageTotalHeight) / 2.0f;

    // 中心のX座標
    centerX = WIN_MAX_X / 2.0f;
    // 中心のY座標
    centerY = WIN_MAX_Y / 2.0f;
}

Stage::~Stage() {}

void Stage::Init(Player &p,int stage) {
    // 重力方向の初期化
    GravityManager::currentDir = GravityDir::Down;

    // ステージの回転角度を初期化
    logic.Init();

    // ステージ番号を格納
    stageNo = stage;

    // 速度リセット
    p.SetVelocityX(0.0f);
    p.SetVelocityY(0.0f);

    // 古いブロックのデータを削除
    blocks.clear();

    // 読み込みファイル名を決定
    std::string fileName = "stage" + std::to_string(stageNo) + ".txt";

    // テキストファイルを開く
    std::ifstream file(fileName);

    if (!file) {
        // ファイルが見つからなかった時のエラー対策で周りを囲むだけにする
        for (int y = 0; y < STAGE_BLOCK_MAX; y++) {
            for (int x = 0; x < STAGE_BLOCK_MAX; x++) {
                if (y == 0 || y == STAGE_BLOCK_MAX - 1 || x == 0 || x == STAGE_BLOCK_MAX - 1) {
                    SetStageBlock(x, y, 0, BlockType::Normal);
                }
            }
        }
        return;
    }

    // テキストファイルから1行ずつ読み込む
    std::string line;
    int y = 0;
    int blockMove = 200.0f; // ブロックの移動量
    while (std::getline(file, line) && y < STAGE_BLOCK_MAX) {
        // 1文字ずつ判定してブロックを設置
        for (int x = 0; x < line.length() && x < STAGE_BLOCK_MAX; x++) {
            char c = line[x];

            switch (c) {
            case '1': // 通常ブロック
                SetStageBlock(x, y, 0, BlockType::Normal);
                break;
            case '2': // ゴール
                SetStageBlock(x, y, 0, BlockType::Goal);
                break;
            case 'G': // 重力ブロック
                SetStageBlock(x, y, 0, BlockType::Gravity);
                break;
            case 'X': // 横に動くブロック
                SetStageBlock(x, y, blockMove, BlockType::MoveX);
                break;
            case 'Y': // 縦に動くブロック
                SetStageBlock(x, y, blockMove, BlockType::MoveY);
                break;
            case 'P': // プレイヤーの初期位置
                p.SetPosition(startX + (x * BLOCK_SIZE), startY + (y * BLOCK_SIZE));
                break;
            default:
                // "0"の場合は空白
                break;
            }
        }
        y++;
    }
}

bool Stage::Update(Player& player) {
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
        return false; // 回転中は以下の処理を行わない
    }

    // --- ここから下は通常時のみ実行 ---
    
    // ゴール判定フラグ
    bool isGoal = false;

    // ブロックの更新ループ
    for (auto& block : blocks) {
        // BlockのUpdateがtrue(ゴール)を返したら、フラグを立てる
        if (block.Update(blocks, player.GetRect())) {
            isGoal = true;
        }
    }

    player.Update(blocks);

    // ゴールに触れたら true を返す
    return isGoal;
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