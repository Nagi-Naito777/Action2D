#include "TitleScene.h"
#include <cmath>

// コンストラクタ
TitleScene::TitleScene(SharedData* data) :
    BaseScene(data),
    m_angle(0.0f), m_timer(0), m_rotateStep(0),
    m_innerX(0.0f), m_innerY(0.0f),
    m_innerVX(0.0f), m_innerVY(0.0f),
    outerHalf(100.0f), innerHalf(5.0f),
    m_escGuard(true) // 最初はガードを有効にしておく
{
}


SceneName TitleScene::Update() {
    // ----------------------------------------------------
    // 回転アニメーションの更新
    // ----------------------------------------------------
    m_timer++;

    // 1サイクルを120フレーム(約2秒)とする
    if (m_timer >= 120) {
        m_timer = 0; // タイマーをリセット
        // 回転ステップを進め、0～3の範囲に収める (3の次は0に戻る)
        m_rotateStep = (m_rotateStep + 1) % 4;
    }

    // 円周率と、現在のステップの基準角度(90度 = π/2)
    const float pi = 3.14159265f;
    float baseAngle = m_rotateStep * (pi / 2.0f);

    if (m_timer < 60) {
        // --- 0～59フレーム: 回転フェーズ ---
        // 進行度を 0.0 ～ 1.0 の割合で計算
        float progress = static_cast<float>(m_timer) / 60.0f;

        // 滑らかに回って止まる計算 (イージング: EaseInOutSine)
        // コサインカーブを利用して、動き出しと止まり際を滑らかにする
        float ease = (1.0f - std::cos(progress * pi)) / 2.0f;

        // 基準角度から、最大90度(π/2)まで滑らかに加算
        m_angle = baseAngle + (pi / 2.0f) * ease;
    }
    else {
        // --- 60～119フレーム: 停止フェーズ ---
        // ぴったり次の90度の位置で固定する(計算誤差をなくす)
        m_angle = baseAngle + (pi / 2.0f);
    }

    // ----------------------------------------------------
    // 内側の四角形の物理演算（重力と当たり判定）
    // ----------------------------------------------------
    // ① 重力の方向を回転に合わせて計算する
    // 外枠が回転している分、中の世界からは「重力の方向が回転している」ように見えます
    float gravity = 0.8f; // 重力の強さ
    float localGravityX = std::sin(m_angle) * gravity;
    float localGravityY = std::cos(m_angle) * gravity;

    // ② 速度に重力を足し、位置を更新する
    m_innerVX += localGravityX;
    m_innerVY += localGravityY;
    m_innerX += m_innerVX;
    m_innerY += m_innerVY;

    // ③ 外枠との当たり判定（ローカル座標系なので、単なる矩形判定で済む）
    float limitPos = outerHalf - innerHalf; // これ以上進むと壁にめり込む位置
    float bounciness = 0.5f;  // 壁に当たった時の反発係数 (0.0〜1.0)

    // X軸の壁の判定
    if (m_innerX > limitPos) {
        m_innerX = limitPos;
        m_innerVX *= -bounciness; // バウンドさせる
    }
    else if (m_innerX < -limitPos) {
        m_innerX = -limitPos;
        m_innerVX *= -bounciness;
    }

    // Y軸の壁の判定
    if (m_innerY > limitPos) {
        m_innerY = limitPos;
        m_innerVY *= -bounciness;
    }
    else if (m_innerY < -limitPos) {
        m_innerY = -limitPos;
        m_innerVY *= -bounciness;
    }

    // ----------------------------------------------------
    // シーン遷移処理
    // ----------------------------------------------------
    
    // 前の画面からのEscキーの押しっぱなしを無効化するガード処理
    if (m_escGuard) {
        // キーが完全に「離された（0になった）」ことを確認したらガードを解除する
        if (CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
            m_escGuard = false;
        }
    }
    else {
        // ガードが解除されたあとに、新しくEscキーが押されたらゲーム終了フラグを立てる
        if (CheckHitKey(KEY_INPUT_ESCAPE)) {
            Game_End = true;
        }
    }

	// エンターキーを押したらステージ選択画面に移行
	if (CheckHitKey(KEY_INPUT_RETURN)) {
		return SceneName::SELECT;
	}
	return SceneName::TITLE;
}

void TitleScene::Draw() const {
    // タイトル文字の描画
    int titleX = 100, titleY = 100;
    DrawFormatStringToHandle(titleX, titleY, Col.GetSky(), Font.GetBig(), _T("ミステリアス\n  リンネキューブ"));

    // ゲーム開始テキスト
    int startX = 100, startY = 600;
    DrawFormatStringToHandle(startX, startY, Col.GetYel(), Font.GetNormal(), _T("Enterキーでゲーム開始"));

    // --- ここから回転する四角形の描画処理 ---
    float centerX = 830.0f;
    float centerY = 630.0f;

    // 共通の回転計算用のサイン・コサイン
    float s = std::sin(m_angle);
    float c = std::cos(m_angle);

    // 【1】外側の四角形の描画
    float outerLocalX[4] = { -outerHalf,  outerHalf,  outerHalf, -outerHalf };
    float outerLocalY[4] = { -outerHalf, -outerHalf,  outerHalf,  outerHalf };
    int outerOutX[4], outerOutY[4];

    for (int i = 0; i < 4; ++i) {
        outerOutX[i] = static_cast<int>(centerX + outerLocalX[i] * c - outerLocalY[i] * s);
        outerOutY[i] = static_cast<int>(centerY + outerLocalX[i] * s + outerLocalY[i] * c);
    }

    DrawQuadrangle(
        outerOutX[0], outerOutY[0], outerOutX[1], outerOutY[1],
        outerOutX[2], outerOutY[2], outerOutX[3], outerOutY[3],
        Col.GetWhi(), FALSE
    );

    // 【2】内側の四角形の描画 (追加部分)
    // 内側の四角形の4つの頂点（ローカル座標での位置を加算）
    float innerLocalX[4] = { m_innerX - innerHalf, m_innerX + innerHalf, m_innerX + innerHalf, m_innerX - innerHalf };
    float innerLocalY[4] = { m_innerY - innerHalf, m_innerY - innerHalf, m_innerY + innerHalf, m_innerY + innerHalf };
    int innerOutX[4], innerOutY[4];

    for (int i = 0; i < 4; ++i) {
        // 外枠と全く同じ回転行列を適用することで、中の空間ごと回転させる
        innerOutX[i] = static_cast<int>(centerX + innerLocalX[i] * c - innerLocalY[i] * s);
        innerOutY[i] = static_cast<int>(centerY + innerLocalX[i] * s + innerLocalY[i] * c);
    }

    // 別色（例として少し明るい青色）で塗りつぶし描画
    // ※お手持ちのColクラスにあれば Col.GetBlue() などに変更してください
    DrawQuadrangle(
        innerOutX[0], innerOutY[0], innerOutX[1], innerOutY[1],
        innerOutX[2], innerOutY[2], innerOutX[3], innerOutY[3],
        Col.GetRed(), TRUE
    );
}