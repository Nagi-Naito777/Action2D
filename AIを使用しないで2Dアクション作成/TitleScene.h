// タイトルシーン管理クラス
#pragma once
#include "BaseScene.h"

class TitleScene :public BaseScene
{
private:
	// メニュー選択用の変数
	int m_menuCursor;   // 0: チュートリアル, 1: ステージ選択
	int prevUpKey;
	int prevDownKey;
	int prevEnterKey;

	// タイトル画面に描画する四角形の回転角度を保持する変数
	float m_angle;

	int m_timer;       // フレームをカウントするタイマーを追加
	int m_rotateStep;  // 何回目の90度回転か(0〜3)を保持する変数を追加

	// --- 内側の四角形のための変数 ---
	float m_innerX, m_innerY;   // 内側の四角形の位置(外枠の中心を0,0としたローカル座標)
	float m_innerVX, m_innerVY; // 内側の四角形の速度(X方向, Y方向)

	float outerHalf; // 外枠の半サイズ
	float innerHalf; // 内枠の半サイズ

	int m_cursorIndex; // 0:チュートリアル, 1:ステージ選択
	bool m_upGuard;    // 上キーの押しっぱなしガード
	bool m_downGuard;  // 下キーの押しっぱなしガード
	bool m_escGuard;   // Escキーの押しっぱなしガード

public:
	TitleScene(SharedData* data);

	void Init() override {};

	SceneName Update() override;

	void Draw() const override;
};