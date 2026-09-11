#pragma once
#include "BaseScene.h"

class SelectScene : public BaseScene {
public:
    SelectScene(SharedData* data);
    ~SelectScene();

    SceneName Update() override;

    void Draw() const override;

private:
    int currentStage;            // 現在選択中のステージ番号

    int currentPage;             // 現在のページ (0始まり。0なら1〜5ステージを表示)
    const int stagesPerPage = 5; // 1ページに表示するステージ数

    // キーのトリガー判定（単発押し判定）用変数
    int prevUpKey = 0;
    int prevDownKey = 0;
    int prevEnterKey = 0;

    // 四角形の回転アニメーション用変数
    bool isRotating = false;    // 回転中フラグ
    float rotationAngle = 0.0f; // 現在の回転角度
    float targetAngle = 0.0f;   // 目標の回転角度
    float rotationSpeed = 0.0f; // 回転速度

    int screenHandle = -1;       // 回転させるための仮想画面ハンドル

    // === プレビュー用の追加変数・関数 ===
    int previewMap[STAGE_BLOCK_MAX][STAGE_BLOCK_MAX];      // 読み込んだステージデータを保持する配列
    int loadedPreviewStage = -1; // 現在読み込まれているステージ番号

    void LoadStagePreview(int stageNo); // テキスト読み込み関数
};