/*
制作開始日:2026年7月13日(月)
【今回のプログラムの目的】
・AIに頼らず短期間で2Dアクションゲームを作成しよう
・短期間でのゲーム作成能力の向上(目標:１週間以内)

そう考えた理由:
AIばかり使ってしまい、自分の強みとしてアピールできる部分がなくなってしまったので、早急に作ろうと考えた為
※ただし、自分自身で理解していて作業が大変なところはAIを使用して効率化を図る(セッターやゲッターなど)

【ゲーム本編の企画内容】
ジャンル:2Dアクションゲーム

使用ツール:Dxライブラリ

コンセプト:回転ギミックを使用した脱出系アクションゲーム(最終目標)

細かいゲーム内容:
・*回転の際にアニメーションを使用して回転ギミックを分かりやすくする
・*重力系や移動ブロックなどの細かいギミックの導入
・
・
※「*」の付いた項目は可能であればの目標となっています。

【デザインパターン】
Stateパターンを採用
*/

#include "DxLib.h"
#include "GameData.h"
#include "Player.h"
#include "PlayerController.h"
#include "Block.h"
#include "Stage.h"

// クラスのインスタンス化
ColorManager Col;
FontManager Font;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    ChangeWindowMode(TRUE);
    if (DxLib_Init() == -1) return -1;
    SetWindowText(_T("スクエアターン"));        // ウィンドウのテキスト変更
    SetGraphMode(WIN_MAX_X, WIN_MAX_Y, 32);     // ウィンドウのサイズ変更
    SetBackgroundColor(0, 0, 0);			// 背景色設定
    SetDrawScreen(DX_SCREEN_BACK);

    // 色とフォントの読み込み
    Col.Init();
    Font.Init();

    // 実体生成
    Player player;
    PlayerController controller(&player);
    Stage stage;
    stage.Init();
    

    while (
        ClearDrawScreen() == 0 &&		// 画面に描かれたものを消去する
        ProcessMessage() == 0 &&        // ウィンドウズのメッセージ処理
        CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        controller.Update();
        stage.Update(player);
        
        stage.Draw();
        player.Draw();

        ScreenFlip();
    }

    DxLib_End();
    return 0;
}