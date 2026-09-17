#include "DebugScene.h"
#include "DxLib.h"
#include <fstream>

DebugScene::DebugScene(SharedData* data)
    : BaseScene(data), isPlayMode(false), currentStageNo(0), selectedBlockChar('1'), mouseClickPrev(0) {
    memset(prevKeys, 0, sizeof(prevKeys));
    controller = new PlayerController(&player);
}

DebugScene::~DebugScene() {}

// 初期化処理
void DebugScene::Init() {
    // SelectSceneから渡されたステージ番号を取得する
    currentStageNo = sharedData->currentStageNo;

    // マップ初期化（すべて空白 '0'）
    for (int y = 0; y < STAGE_BLOCK_MAX; ++y) {
        for (int x = 0; x < STAGE_BLOCK_MAX; ++x) {
            stageMap[y][x] = 0;
        }
    }
    // 周囲を囲む壁（例として1を置くなど）
    isPlayMode = false;

    selectedBlockChar = '1'; // 初期選択は通常ブロック

    // 起動時に現在のステージファイルを読み込む（存在する場合）
    LoadDebugStage("data/stage" + std::to_string(currentStageNo) + ".txt");
}

// 更新処理
SceneName DebugScene::Update() {
    // 移動処理
    controller->Update();

    char currentKeys[256];
    GetHitKeyStateAll(currentKeys);

    if (!isPlayMode) {
        // --- 1. 配置ブロックの切り替え (数字キー 1~7) ---
        if (currentKeys[KEY_INPUT_1]) selectedBlockChar = '1'; // 通常
        if (currentKeys[KEY_INPUT_2]) selectedBlockChar = '2'; // ゴール
        if (currentKeys[KEY_INPUT_3]) selectedBlockChar = 'G'; // 重力
        if (currentKeys[KEY_INPUT_4]) selectedBlockChar = 'S'; // スイッチ
        if (currentKeys[KEY_INPUT_5]) selectedBlockChar = 'X'; // 横移動
        if (currentKeys[KEY_INPUT_6]) selectedBlockChar = 'Y'; // 縦移動
        if (currentKeys[KEY_INPUT_7]) selectedBlockChar = 'P'; // プレイヤー初期位置

        // --- 2. ステージ番号の変更操作 (↑/↓キー) ---
        if (currentKeys[KEY_INPUT_UP] && !prevKeys[KEY_INPUT_UP]) {
            currentStageNo = (currentStageNo < STAGE_MAX) ? currentStageNo + 1 : 1;
            LoadDebugStage("data/stage" + std::to_string(currentStageNo) + ".txt");
        }
        if (currentKeys[KEY_INPUT_DOWN] && !prevKeys[KEY_INPUT_DOWN]) {
            currentStageNo = (currentStageNo > 1) ? currentStageNo - 1 : STAGE_MAX;
            LoadDebugStage("data/stage" + std::to_string(currentStageNo) + ".txt");
        }

        // --- 3. ステージの保存 (Sキー) ---
        if (currentKeys[KEY_INPUT_S] && !prevKeys[KEY_INPUT_S]) {
            SaveDebugStage("data/stage" + std::to_string(currentStageNo) + ".txt");

            // 保存した後にステージ選択画面へ移行するシーン名を返す
            return SceneName::SELECT;
        }

        // --- 4. マウスによる配置・削除処理 ---
        int mouseX, mouseY;
        GetMousePoint(&mouseX, &mouseY);
        int mouseInput = GetMouseInput();

        float stageTotalWidth = STAGE_BLOCK_MAX * BLOCK_SIZE;
        float stageTotalHeight = STAGE_BLOCK_MAX * BLOCK_SIZE;
        int startX = (WIN_MAX_X - (int)stageTotalWidth) / 2;
        int startY = (WIN_MAX_Y - (int)stageTotalHeight) / 2;

        int gridX = (mouseX - startX) / BLOCK_SIZE;
        int gridY = (mouseY - startY) / BLOCK_SIZE;

        if (gridX >= 0 && gridX < STAGE_BLOCK_MAX && gridY >= 0 && gridY < STAGE_BLOCK_MAX) {
            // 左クリック: 設置
            if (mouseInput & MOUSE_INPUT_LEFT) {
                // プレイヤー位置('P')を設置する場合は、既存の'P'を消去（重複防止）
                if (selectedBlockChar == 'P') {
                    for (int y = 0; y < STAGE_BLOCK_MAX; ++y) {
                        for (int x = 0; x < STAGE_BLOCK_MAX; ++x) {
                            if (stageMap[y][x] == 'P') stageMap[y][x] = '0';
                        }
                    }
                }
                // ゴールブロックも同じく重複防止する
                if (selectedBlockChar == '2') {
                    for (int y = 0; y < STAGE_BLOCK_MAX; ++y) {
                        for (int x = 0; x < STAGE_BLOCK_MAX; ++x) {
                            if (stageMap[y][x] == '2') stageMap[y][x] = '0';
                        }
                    }
                }
                stageMap[gridY][gridX] = selectedBlockChar;
            }
            // 右クリック: 削除
            else if (mouseInput & MOUSE_INPUT_RIGHT) {
                stageMap[gridY][gridX] = '0';
            }
        }
    }

    // --- 5. エディット ⇔ プレイモード切り替え (Pキー) ---
    if (currentKeys[KEY_INPUT_P] && !prevKeys[KEY_INPUT_P]) {
        isPlayMode = !isPlayMode;

        if (isPlayMode) {
            // 現在のエディット状態を保存してStageクラスに読み込ませる
            std::string fileName = "data/stage" + std::to_string(currentStageNo) + ".txt";
            SaveDebugStage(fileName);
            stage.Init(player, currentStageNo);
        }
    }

    if (isPlayMode) {
        // --- プレイモード中の操作 ---

        // スペースキーが押されたら、ステージとプレイヤーを初期状態にリセット
        if (currentKeys[KEY_INPUT_SPACE] && !prevKeys[KEY_INPUT_SPACE]) {
            stage.Init(player, currentStageNo);
        }

        // ステージとプレイヤーの更新
        stage.Update(player);
    }

    memcpy(prevKeys, currentKeys, sizeof(currentKeys));
    return SceneName::DEBUG;
}

// 描画処理
void DebugScene::Draw() const {
    float stageTotalWidth = STAGE_BLOCK_MAX * BLOCK_SIZE;
    float stageTotalHeight = STAGE_BLOCK_MAX * BLOCK_SIZE;
    int startX = (WIN_MAX_X - (int)stageTotalWidth) / 2;
    int startY = (WIN_MAX_Y - (int)stageTotalHeight) / 2;
    int x = 10; // フォント描画用

    if (!isPlayMode) {
        // マップ上のブロックを描画
        for (int y = 0; y < STAGE_BLOCK_MAX; y++) {
            for (int x = 0; x < STAGE_BLOCK_MAX; x++) {
                int px = startX + x * BLOCK_SIZE;
                int py = startY + y * BLOCK_SIZE;
                char type = stageMap[y][x];

                // ブロックの種類に応じた色分け描画
                if (type == '1') { // 通常
                    DrawBox(px, py, px + BLOCK_SIZE, py + BLOCK_SIZE, Col.GetGra(), TRUE);
                }
                else if (type == '2') { // ゴール
                    DrawBox(px, py, px + BLOCK_SIZE, py + BLOCK_SIZE, Col.GetYel(), TRUE);
                }
                else if (type == 'G') { // 重力
                    DrawBox(px, py, px + BLOCK_SIZE, py + BLOCK_SIZE, Col.GetGre(), TRUE);
                }
                else if (type == 'S') { // スイッチ
                    DrawBox(px, py, px + BLOCK_SIZE, py + BLOCK_SIZE, Col.GetSky(), TRUE);
                }
                else if (type == 'X') { // 横移動
                    DrawBox(px, py, px + BLOCK_SIZE, py + BLOCK_SIZE, Col.GetWhi(), TRUE);
                }
                else if (type == 'Y') { // 縦移動
                    DrawBox(px, py, px + BLOCK_SIZE, py + BLOCK_SIZE, Col.GetWhi(), TRUE);
                }
                else if (type == 'P') { // プレイヤー
                    DrawBox(px, py, px + BLOCK_SIZE, py + BLOCK_SIZE, Col.GetRed(), TRUE);
                }

                // ブロックの識別記号を描画（文字で視認性を確保）
                if (type != '0') {
                    DrawFormatString(px + 10, py + 8, GetColor(0, 0, 0), "%c", type);
                }

                // グリッド枠線
                DrawBox(px, py, px + BLOCK_SIZE, py + BLOCK_SIZE, GetColor(60, 60, 60), FALSE);
            }
        }

        // マウスホバー位置のハイライト処理
        int mouseX, mouseY;
        GetMousePoint(&mouseX, &mouseY);
        int gridX = (mouseX - startX) / BLOCK_SIZE;
        int gridY = (mouseY - startY) / BLOCK_SIZE;

        if (gridX >= 0 && gridX < STAGE_BLOCK_MAX && gridY >= 0 && gridY < STAGE_BLOCK_MAX) {
            int px = startX + gridX * BLOCK_SIZE;
            int py = startY + gridY * BLOCK_SIZE;
            DrawBox(px, py, px + BLOCK_SIZE, py + BLOCK_SIZE, GetColor(255, 0, 0), FALSE);
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
            DrawBox(px + 2, py + 2, px + BLOCK_SIZE - 2, py + BLOCK_SIZE - 2, GetColor(255, 0, 0), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }

        // UI表示
        DrawFormatStringToHandle(x, 10, Col.GetWhi(), Font.GetNormal(), "編集対象: stage%d.txt (↑/↓で切替)", currentStageNo);
        DrawFormatStringToHandle(x, 50, Col.GetYel(), Font.GetNormal(), "選択中ブロック: [%c]", selectedBlockChar);
        DrawFormatStringToHandle(x, 100, Col.GetWhi(), Font.GetStageMake(),
            "【数字キーで変更】\n1:通常\n2:ゴール\n3:重力(G)\n4:スイッチ(S)\n5:横移動(X)\n6:縦移動(Y)\n7:プレイヤー(P)", Col.GetWhi());
        DrawFormatStringToHandle(x, 600, Col.GetWhi(), Font.GetStageMake(),
            "【操作方法】\n左クリック:設置\n右クリック:削除\nS:保存＆タイトルへ\nP:テストプレイ", Col.GetWhi());
    }
    else {
        // テストプレイ中の文字描画（横中央揃え）
        const TCHAR* debugStr = _T("[PLAY MODE] Stage %d テスト中 | Pキー: エディットに戻る");
        int debugWidth = GetDrawFormatStringWidthToHandle(Font.GetStageMake(), debugStr);
        int debugX = (WIN_MAX_X - debugWidth) / 2;
        int debugY = 700;
        DrawFormatStringToHandle(debugX, debugY, Col.GetGre(), Font.GetStageMake(), debugStr, currentStageNo);

        stage.Draw(player);
    }
}

// ファイル書き出し処理
void DebugScene::SaveDebugStage(const std::string& filename) const {
    std::ofstream ofs(filename);
    if (!ofs) return;

    for (int y = 0; y < STAGE_BLOCK_MAX; ++y) {
        for (int x = 0; x < STAGE_BLOCK_MAX; ++x) {
            ofs << stageMap[y][x];
        }
        ofs << "\n";
    }
    ofs.close();
}

// ファイル読み込み処理 (既存ファイルを編集画面に復元)
void DebugScene::LoadDebugStage(const std::string& filename) {
    // 配列初期化
    for (int y = 0; y < STAGE_BLOCK_MAX; ++y) {
        for (int x = 0; x < STAGE_BLOCK_MAX; ++x) {
            stageMap[y][x] = '0';
        }
    }

    std::ifstream ifs(filename);
    if (!ifs) return;

    std::string line;
    int y = 0;
    while (std::getline(ifs, line) && y < STAGE_BLOCK_MAX) {
        for (int x = 0; x < line.length() && x < STAGE_BLOCK_MAX; ++x) {
            stageMap[y][x] = line[x];
        }
        y++;
    }
    ifs.close();
}