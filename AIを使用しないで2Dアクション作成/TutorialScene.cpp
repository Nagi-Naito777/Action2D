#include "TutorialScene.h"
#include "DxLib.h"

TutorialScene::TutorialScene(SharedData* data) : BaseScene(data) {
	controller = new PlayerController(&player);
	currentStep = 1;
}

TutorialScene::~TutorialScene() {
	delete controller;
}

void TutorialScene::Init() {
	// チュートリアル開始時はクリアフラグをリセット
	sharedData->isClear = false;

	// チュートリアル用のステージ初期化関数を呼ぶ
	stage.InitTutorial(player, currentStep);

    // 現在のステップに対応した手順(タスク)を読み込み
    LoadTasks(currentStep);
}

// チュートリアルの手順を定義する処理
void TutorialScene::LoadTasks(int step) {
    tasks.clear();
    currentPhase = 0; // 手順を初期化
    waitTimer = 0;    // タイマーも初期化

    switch (step)
    {
    case 1:
        // 【Step1】フェーズ0: 左に90°回転して奈落に落ちる
        tasks.push_back({
            "左回転キー(←)を押してみよう！",
            ACTION_ROTATE_L90, // 左回転のみ許可
            [this]() { return CheckHitKey(KEY_INPUT_LEFT) != 0; }
            });

        // 【Step1】フェーズ1: 奈落に落ちるのを見る（2秒待機）
        tasks.push_back({
            "左回転キー(←)を押してみよう！",
            ACTION_NONE, // 全操作をロック
            [this]() {
                waitTimer++;
                return waitTimer >= 120; // 60FPS想定で2秒経過したら次へ
            }
            });

        // 【Step1】フェーズ2: 奈落に落ちてしまった！（2秒待機）
        tasks.push_back({
            "あ！奈落に落ちてしまった！",
            ACTION_NONE, // 全操作をロック
            [this]() {
                waitTimer++;
                return waitTimer >= 120; // 60FPS想定で2秒経過したら次へ
            }
            });

        // 【Step1】フェーズ3: ステージリセット
        tasks.push_back({
            "リセットキー(Space)を押してやり直そう！",
            ACTION_RESET, // リセットのみ許可
            [this]() {
                // 初期化自体はUpdateの末尾で共通して行うため、ここでは判定だけ返す
                return CheckHitKey(KEY_INPUT_SPACE) != 0;
            }
            });

        // 【Step1】フェーズ4: 右回転と移動でクリア
        tasks.push_back({
            "今度は右回転キー(→)や移動キー(A, D)でゴールを目指そう！",
            // プレイヤー移動キー(左右)と右回転キーのロックを解除
            ACTION_ROTATE_R90 | ACTION_MOVE_RIGHT | ACTION_MOVE_LEFT,
            [this]() { return false; } // ゴール到達でクリアとするため、ここでは常にfalse
            });
        break;

    case 2:
        // 【Step2】フェーズ0: 左回転バージョン（奈落の処理は無し）
        tasks.push_back({
            "左回転キー(←)や移動キー(A, D)を使ってゴールしよう！",
            ACTION_ROTATE_L90 | ACTION_MOVE_RIGHT | ACTION_MOVE_LEFT,
            [this]() { return false; }
            });
        break;

    case 3:
        // 【Step3】フェーズ0: 180°回転でクリア
        tasks.push_back({
            "180°回転キー(↑)を使って、ゴールを目指そう！",
            ACTION_ROTATE_180 | ACTION_MOVE_RIGHT | ACTION_MOVE_LEFT,
            [this]() { return false; }
            });
        break;

    default:
        break;
    }

    // 初回の手順の操作制限を適用する
    ApplyAllowedActions();
}

// 各クラスへ操作許可フラグを伝える
void TutorialScene::ApplyAllowedActions() {
    if (tasks.empty() || currentPhase >= tasks.size()) return;

    unsigned int currentAllowed = tasks[currentPhase].allowedActions;

    // StageとPlayerControllerクラスに、現在の許可フラグをセットする
    controller->SetAllowedActions(currentAllowed);
    stage.SetAllowedActions(currentAllowed);
}

SceneName TutorialScene::Update() {
    // 移動処理
    controller->Update();

    // ステージ処理
    bool isGoalReached = stage.Update(player);

    // チュートリアルのフェーズ(手順)進行処理
    if (!tasks.empty() && currentPhase < tasks.size()) {
        // 現在の手順の達成条件を満たしたら次の手順へ
        if (tasks[currentPhase].checkClear()) {
            currentPhase++;
            waitTimer = 0;         // フェーズが切り替わったらタイマーをリセット
            ApplyAllowedActions(); // アクション制限を更新
        }
    }

    // 1項目（1ステージ）達成時の処理
    if (isGoalReached) {
        currentStep++;

        // 全ステップ完了した場合はクリアとしてリザルトへ遷移
        if (currentStep > MAX_TUTORIAL_STEPS) {
            sharedData->isClear = true;
            return SceneName::RESULT;
        }
        else {
            // 次のステップを読み込む(LoadTasksも呼ばれる)
            Init();
        }
    }

    // ESCキーでタイトルに戻る
    if (CheckHitKey(KEY_INPUT_ESCAPE)) {
        return SceneName::TITLE;
    }

    // スペースキーでステージリセット
    // ※押しっぱなしで毎フレームリセットされるのを防ぐための簡易トリガー判定
    static int prevSpace = 0;
    int currentSpace = CheckHitKey(KEY_INPUT_SPACE);
    bool isTriggerSpace = (currentSpace == 1 && prevSpace == 0);
    prevSpace = currentSpace;

    if (isTriggerSpace) {
        // 現在が「リセットを促すタスク（Step1のフェーズ2）」の場合は、
        // タスク側(checkClear)で処理してフェーズを進めるため、ここでは無視する
        if (!tasks.empty() && currentPhase < tasks.size() && tasks[currentPhase].allowedActions == ACTION_RESET) {
            // 何もしない
        }
        else {
            // 自発的リセット時はInit()を呼ばず、ステージとプレイヤーの状態だけ初期化
            // これにより、チュートリアルの進行(currentPhase)がリセットされなくなります。
            stage.InitTutorial(player, currentStep);
        }
    }

    return SceneName::TUTORIAL;
}

void TutorialScene::Draw() const {
    stage.Draw(player);

    // 現在の手順のテキストを描画
    if (!tasks.empty() && currentPhase < tasks.size()) {
        int clearWidth = GetDrawFormatStringWidthToHandle(Font.GetNormal(), tasks[currentPhase].message.c_str());
        int clearX = (WIN_MAX_X - clearWidth) / 2;
        int clearY = 700;

        DrawFormatStringToHandle(clearX, clearY, Col.GetYel(), Font.GetNormal(), tasks[currentPhase].message.c_str());
    }
}