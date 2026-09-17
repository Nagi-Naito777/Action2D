#include "Stage.h"
#include "Collision.h"
#include "Block.h"
#include "Player.h"
#include "StageLogic.h"
#include "Tutorial.h"
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

// ステージファイルの共通読み込み関数
void Stage::LoadStageFile(Player& p, const std::string& fileName) {
    // 重力方向の初期化
    GravityManager::currentDir = GravityDir::Down;

    // ステージの回転角度を初期化
    logic.Init();

    // 速度リセット
    p.SetVelocityX(0.0f);
    p.SetVelocityY(0.0f);

    // 古いブロックのデータを削除
    blocks.clear();

    // ギミック用変数のリセット
    hasSwitch = false;
    goalBlockIndex = -1;

    // テキストファイルを開く
    std::ifstream file(fileName);

    if (!file) {
        // ファイルが見つからなかった場合のエラー対策（枠で囲む）
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
    int blockMove = 200.0f;
    while (std::getline(file, line) && y < STAGE_BLOCK_MAX) {
        for (int x = 0; x < line.length() && x < STAGE_BLOCK_MAX; x++) {
            char c = line[x];

            switch (c) {
            case '1':
                SetStageBlock(x, y, 0, BlockType::Normal);
                break;
            case '2':
                SetStageBlock(x, y, 0, BlockType::Goal);
                goalBlockIndex = static_cast<int>(blocks.size()) - 1;
                break;
            case 'G':
                SetStageBlock(x, y, 0, BlockType::Gravity);
                break;
            case 'S':
                SetStageBlock(x, y, 0, BlockType::Switch);
                hasSwitch = true;
                break;
            case 'X':
                SetStageBlock(x, y, blockMove, BlockType::MoveX);
                break;
            case 'Y':
                SetStageBlock(x, y, blockMove, BlockType::MoveY);
                break;
            case 'P':
                p.SetPosition(startX + (x * BLOCK_SIZE), startY + (y * BLOCK_SIZE));
                break;
            default:
                break;
            }
        }
        y++;
    }
}

// 通常ステージの初期化
void Stage::Init(Player& p, int stage) {
    stageNo = stage;
    std::string fileName = "data/stage" + std::to_string(stageNo) + ".txt";
    LoadStageFile(p, fileName);
}

// チュートリアルステージの初期化
void Stage::InitTutorial(Player& p, int tutorialNo) {
    stageNo = tutorialNo;
    std::string fileName = "data/tutorial" + std::to_string(tutorialNo) + ".txt"; // 例: tutorial1.txt
    LoadStageFile(p, fileName);
}

bool Stage::Update(Player& player) {
    int currentKeyRight = 0;
    int currentKeyLeft = 0;
    int currentKeyUp = 0;

    // 許可されているアクションのみ、キーの入力状態を取得する
    if (allowedActions & ACTION_ROTATE_R90) {
        currentKeyRight = CheckHitKey(KEY_INPUT_RIGHT); // または Dキー
    }
    if (allowedActions & ACTION_ROTATE_L90) {
        currentKeyLeft = CheckHitKey(KEY_INPUT_LEFT);   // または Aキー
    }
    if (allowedActions & ACTION_ROTATE_180) {
        currentKeyUp = CheckHitKey(KEY_INPUT_UP);       // または Wキー
    }

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
    
    // スイッチブロックのギミック処理
    if (hasSwitch && goalBlockIndex != -1) {
        // 全てのスイッチが押されているかフラグ
        bool allSwitchesPressed = true;

        // スイッチブロックを探して重なり判定を行う
        for (auto& switchBlock : blocks) {
            // もし種類がスイッチブロックなら
            if (switchBlock.GetType() == BlockType::Switch) {
                // このスイッチが押されているかフラグ
                bool thisSwitchPressed = false;
                // プレイヤーが重なっているか判定
                if (IsHitAABB(switchBlock.GetRect(), player.GetRect())) {
                    thisSwitchPressed = true;
                    break;
                }
                else {
                    // プレイヤーが重なっていなければ、他のブロックが重なっているか判定
                    for (auto& otherBlock : blocks) {
                        // 自分自身、または「無効なブロック」との判定はスキップ
                        if (&switchBlock == &otherBlock || !otherBlock.IsActive()) {
                            continue;
                        }

                        // AABB判定を行う
                        if (IsHitAABB(switchBlock.GetRect(), otherBlock.GetRect())) {
                            thisSwitchPressed = true;
                        }
                    }
                }

                // もし「このスイッチ」が押されていなかったら
                if (!thisSwitchPressed) {
                    allSwitchesPressed = false; // 全てのスイッチが押されているわけではない
                    break; // 1つでも押されていないスイッチを見つけたら、これ以上の確認は不要なのでループを抜ける
                }
            }
        }

        // スイッチが押されていればゴールを出現させ、離れれば隠す
        blocks[goalBlockIndex].SetActive(allSwitchesPressed);
    }

    // ゴール判定フラグ
    bool isGoal = false;

    // ブロックの更新ループ
    for (auto& block : blocks) {
        if (block.IsActive()) {
            // BlockのUpdateがtrue(ゴール)を返したら、フラグを立てる
            if (block.Update(blocks, player.GetRect())) {
                isGoal = true;
            }
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