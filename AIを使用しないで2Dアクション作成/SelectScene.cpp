#include "SelectScene.h"
#include <math.h>
#include <stdio.h>
#include <fstream>
#include <string>

// 四角形のサイズ定数
const int RECT_WIDTH = 750;
const int RECT_HEIGHT = 750;

// コンストラクタで初期化 & 仮想画面の作成
SelectScene::SelectScene(SharedData* data) : BaseScene(data) {
    currentStage = 1;
    currentPage = 0;

    // ビット論理和を使用してWASDキーにも対応
    prevUpKey = CheckHitKey(KEY_INPUT_UP) | CheckHitKey(KEY_INPUT_W);
    prevDownKey = CheckHitKey(KEY_INPUT_DOWN) | CheckHitKey(KEY_INPUT_S);
    prevLeftKey = CheckHitKey(KEY_INPUT_LEFT) | CheckHitKey(KEY_INPUT_A);
    prevRightKey = CheckHitKey(KEY_INPUT_RIGHT) | CheckHitKey(KEY_INPUT_D);
    prevEnterKey = CheckHitKey(KEY_INPUT_RETURN);

    isRotating = false;
    rotationAngle = 0.0f;
    targetAngle = 0.0f;
    rotationSpeed = 0.0f;

    // 四角形の背景と枠を描画するための仮想画面（オフスクリーン）を作成
    screenHandle = MakeScreen(RECT_WIDTH, RECT_HEIGHT, TRUE);

    // 初期ステージのプレビューを読み込み
    LoadStagePreview(currentStage);
}

// デストラクタで仮想画面を解放
SelectScene::~SelectScene() {
    if (screenHandle != -1) {
        DeleteGraph(screenHandle);
    }
}

// テキストファイルから文字データをそのままプレビュー用に読み込む関数
void SelectScene::LoadStagePreview(int stageNo) {
    // 既に同じステージを読み込んでいるなら再読み込みしない
    if (loadedPreviewStage == stageNo) return;

    // マップ配列を一旦クリア（すべて '0' とする）
    for (int y = 0; y < STAGE_BLOCK_MAX; ++y) {
        for (int x = 0; x < STAGE_BLOCK_MAX; ++x) {
            previewMap[y][x] = '0';
        }
    }

    // ファイル名を組み立て
    std::string fileName = "stage" + std::to_string(stageNo) + ".txt";
    std::ifstream file(fileName);

    if (file) {
        std::string line;
        int y = 0;
        while (std::getline(file, line) && y < STAGE_BLOCK_MAX) {
            for (int x = 0; x < line.length() && x < STAGE_BLOCK_MAX; x++) {
                char c = line[x];
                if (c != '\r' && c != '\n') {
                    previewMap[y][x] = c;
                }
            }
            y++;
        }
        loadedPreviewStage = stageNo;
    }
    else {
        // ファイルが見つからない場合のフォールバック
        loadedPreviewStage = stageNo;
    }
}

void SelectScene::ChangeStage(int amount) {
    int nextStage = currentStage + amount;

    // --- ステージ番号の限界突破を防ぐ（補正） ---
    if (nextStage < 1) {
        nextStage = 1;
    }
    if (nextStage > STAGE_MAX) {
        nextStage = STAGE_MAX;
    }

    // --- ステージが実際に変わった場合のみ実行 ---
    if (currentStage != nextStage) {
        currentStage = nextStage;
        LoadStagePreview(currentStage); // プレビューを更新

        // 新しいステージが属するページ番号を計算
        int newPage = (currentStage - 1) / stagesPerPage;

        // --- ページ遷移が発生した場合のみ回転アニメーションをセット ---
        if (newPage > currentPage) {
            currentPage = newPage;
            isRotating = true;
            targetAngle -= DX_PI_F / 2.0f;
            rotationSpeed = -(DX_PI_F / 2.0f) / 20.0f;
        }
        else if (newPage < currentPage) {
            currentPage = newPage;
            isRotating = true;
            targetAngle += DX_PI_F / 2.0f;
            rotationSpeed = (DX_PI_F / 2.0f) / 20.0f;
        }
    }
}

SceneName SelectScene::Update() {
    // ビット論理和を使用してWASDキーにも対応
    int upKey = CheckHitKey(KEY_INPUT_UP) | CheckHitKey(KEY_INPUT_W);
    int downKey = CheckHitKey(KEY_INPUT_DOWN) | CheckHitKey(KEY_INPUT_S);
    int leftKey = CheckHitKey(KEY_INPUT_LEFT) | CheckHitKey(KEY_INPUT_A);
    int rightKey = CheckHitKey(KEY_INPUT_RIGHT) | CheckHitKey(KEY_INPUT_D);
    int enterKey = CheckHitKey(KEY_INPUT_RETURN);

    if (CheckHitKey(KEY_INPUT_SPACE) == 1) {
        sharedData->currentStageNo = 99;
        return SceneName::DEBUG;
    }
    if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) {
        return SceneName::TITLE;
    }

    // ====== 回転アニメーション中の処理 ======
    if (isRotating) {
        rotationAngle += rotationSpeed;

        // 目標の角度に到達したら回転終了
        if ((rotationSpeed > 0 && rotationAngle >= targetAngle) ||
            (rotationSpeed < 0 && rotationAngle <= targetAngle)) {
            rotationAngle = targetAngle;
            isRotating = false;
        }
    }
    // ====== アニメーションしていない時だけ操作可能 ======
    else {

        // 下キー：1ステージ進む
        if (downKey == 1 && prevDownKey == 0)   ChangeStage(1);
        // 上キー：1ステージ戻る
        if (upKey == 1 && prevUpKey == 0)       ChangeStage(-1);
        // 右キー：1ページ分（stagesPerPage）一気に進む
        if (rightKey == 1 && prevRightKey == 0) ChangeStage(stagesPerPage);
        // 左キー：1ページ分（stagesPerPage）一気に戻る
        if (leftKey == 1 && prevLeftKey == 0)   ChangeStage(-stagesPerPage);

        // エンターキーで決定
        if (enterKey == 1 && prevEnterKey == 0) {
            sharedData->currentStageNo = currentStage;
            return SceneName::PLAY;
        }
    }

    // 次フレームのためのキー状態更新
    prevUpKey = upKey;
    prevDownKey = downKey;
    prevLeftKey = leftKey;
    prevRightKey = rightKey;
    prevEnterKey = enterKey;

    return SceneName::SELECT;
}

void SelectScene::Draw() const {
    float rectCenterX = 75.0f;
    float rectCenterY = WIN_MAX_Y / 2.0f; // 画面の縦中心

    // --- 仮想画面（オフスクリーン）には四角形の「背景と枠線」だけを描画する ---
    int oldScreen = GetDrawScreen(); // 現在の描画先を保存
    SetDrawScreen(screenHandle);     // 描画先を仮想画面に変更
    ClearDrawScreen();               // 仮想画面内をクリア（透明化）

    float localLeft = 0.0f;
    float localTop = 0.0f;
    float localRight = (float)RECT_WIDTH;
    float localBottom = (float)RECT_HEIGHT;

    // 背景と枠線を描画（これだけが回転する対象になります）
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    DrawBox(localLeft, localTop, localRight, localBottom, Col.GetSky(), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    DrawBox(localLeft, localTop, localRight, localBottom, Col.GetGra(), FALSE);

    // --- 描画先を元の画面に戻す ---
    SetDrawScreen(oldScreen);

    // --- 仮想画面を指定位置に「90度単位で回転させて」描画する ---
    DrawRotaGraphF(rectCenterX, rectCenterY, 1.0f, rotationAngle, screenHandle, TRUE, FALSE);

    // ステージ選択フォント描画
    int stageFontX = 575, stageFontY = 40;
    DrawFormatStringToHandle(stageFontX, stageFontY,
        Col.GetWhi(), Font.GetBig(), _T("ステージ選択"));
    DrawFormatStringToHandle(stageFontX, stageFontY + 75,
        Col.GetWhi(), Font.GetNormal(), _T("SPACE:ステージ開発モード"));
    

    // --- ページ番号の表示 ---
    int pageX = 690, pageY = 750;
    int maxPage = (STAGE_MAX + stagesPerPage - 1) / stagesPerPage; // 最大ページ数計算
    DrawFormatStringToHandle(pageX, pageY, Col.GetWhi(), Font.GetNormal(), _T("< %2d/%2d >"), currentPage + 1, maxPage);

    // --- フォント（文字）の描画 ---
    // 四角形が回転している間（isRotatingがtrue）はフォントを表示しないようにガード
    if (!isRotating) {
        int startStageNo = currentPage * stagesPerPage + 1;

        float textBaseX = 90.0f;
        float textBaseY = 240.0f;

        for (int i = 0; i < stagesPerPage; ++i) {
            int stageNo = startStageNo + i;
            if (stageNo > STAGE_MAX) break;

            unsigned int color = (stageNo == currentStage) ? Col.GetYel() : Col.GetWhi();
            const TCHAR* cursor = (stageNo == currentStage) ? _T("<") : _T("  ");

            float textX = textBaseX;
            float textY = textBaseY + (i * 70.0f);

            DrawFormatStringToHandle(textX, textY, color, Font.GetBig(), _T("ステージ%3d %s"), stageNo, cursor);
        }

        // --- ★右側の余把にステージのブロックプレビューを描画する ---
        float previewStartX = 596.0f; // 右側余白の開始X座標
        float previewStartY = 246.0f; // 右側余白の開始Y座標
        float blockSize = 14.0f;      // ミニブロックの1マスのサイズ（ピクセル）

        for (int y = 0; y < STAGE_BLOCK_MAX; ++y) {
            for (int x = 0; x < STAGE_BLOCK_MAX; ++x) {
                float bx = previewStartX + (x * blockSize);
                float by = previewStartY + (y * blockSize);

                char c = previewMap[y][x];

                // 通路の背景をまず描画
                DrawBox(bx, by, bx + blockSize, by + blockSize, GetColor(30, 30, 50), TRUE);
                DrawBox(bx, by, bx + blockSize, by + blockSize, GetColor(60, 60, 80), FALSE);

                // 文字（ブロックの種類）に合わせて色を切り替え
                switch (c) {
                case '1': // 通常ブロック (グレー)
                    DrawBox(bx, by, bx + blockSize, by + blockSize, Col.GetGra(), TRUE);
                    break;
                case 'G': // 重力ブロック (グリーン)
                    DrawBox(bx, by, bx + blockSize, by + blockSize, Col.GetGre(), TRUE);
                    break;
                case 'S': // スイッチブロック (半透明スカイブルー)
                    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
                    DrawBox(bx, by, bx + blockSize, by + blockSize, Col.GetSky(), TRUE);
                    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
                    break;
                case 'X':
                case 'Y': // 動くブロック (ホワイト)
                    DrawBox(bx, by, bx + blockSize, by + blockSize, Col.GetWhi(), TRUE);
                    break;
                case '2': // ゴール (イエロー)
                    DrawBox(bx, by, bx + blockSize, by + blockSize, Col.GetYel(), TRUE);
                    break;
                case 'P': // プレイヤー初期位置 (赤い四角)
                    DrawBox(bx, by, bx + blockSize, by + blockSize, GetColor(255, 0, 0), TRUE);
                    break;
                default:
                    // '0' またはその他の文字は何もしない（背景のみ）
                    break;
                }
            }
        }
    }
}