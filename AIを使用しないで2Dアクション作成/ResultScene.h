// リザルト画面クラス
#pragma once
#include "BaseScene.h"
#include <vector>

// 紙吹雪1枚分のデータを管理する構造体
struct Confetti {
	float x, y;          // 現在の座標
	float baseX;         // 左右の揺れの基準となるX座標
	float vy;            // 落下速度
	float size;          // 紙吹雪のサイズ(半分)
	float angle;         // 現在の回転角度
	float angleSpeed;    // 回転スピード
	unsigned int color;  // 色
};

class ResultScene :public BaseScene
{
private:
	// リザルト画面に描画する四角形の回転角度を保持する変数
	float m_angle;
	float m_angleRev;	// 逆回転用

	int m_timer;       // フレームをカウントするタイマーを追加
	int m_rotateStep;  // 何回目の90度回転か(0〜3)を保持する変数を追加
	int m_rotateFrames; // 回転にかかるフレーム数
	int m_stopFrames;   // 止まっているフレーム数

	// 正回転する箱の中の赤い四角の変数
	float m_innerX, m_innerY;
	float m_innerVX, m_innerVY;

	// 逆回転する箱の中の赤い四角の変数
	float m_innerRevX, m_innerRevY;
	float m_innerRevVX, m_innerRevVY;

	float outerHalf; // 外枠の半サイズ
	float innerHalf; // 内枠の半サイズ

	// 任意の四角形を回転させて描画する便利関数
	void DrawRotatedBox(float centerX, float centerY, float offsetX, float offsetY, 
		float halfSize, float angle, unsigned int color, int fillFlag) const;

	std::vector<Confetti> m_confettis; // 紙吹雪のリスト
	void UpdateConfetti();             // 紙吹雪の計算用メソッド
	void DrawConfetti() const;         // 紙吹雪の描画用メソッド

public:
	// コンストラクタ
	ResultScene(SharedData* data);

	// デストラクタ
	~ResultScene();

	void Init() override;
	SceneName Update() override;
	void Draw() const override;
};

