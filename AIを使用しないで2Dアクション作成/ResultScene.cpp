#include "ResultScene.h"
#include "GameData.h"
#include "DxLib.h"

// コンストラクタ
ResultScene::ResultScene(SharedData* data) :
    BaseScene(data),
    m_angle(0.0f), 
    m_angleRev(0.0f),    // 逆回転用角度の初期化
    m_timer(0),
    m_rotateStep(0),
    m_rotateFrames(30),  // 回転にかける時間（60フレーム = 1秒）
    m_stopFrames(30),    // 止まっている時間（60フレーム = 1秒）
    m_innerX(0.0f), m_innerY(0.0f),
    m_innerVX(0.0f), m_innerVY(0.0f),
    m_innerRevX(0.0f), m_innerRevY(0.0f),
    m_innerRevVX(0.0f), m_innerRevVY(0.0f),
    outerHalf(100.0f), innerHalf(5.0f)
{
}

// デストラクタ
ResultScene::~ResultScene(){}

void ResultScene::Init() {
    m_confettis.clear();

    // =========================================================
    // ▼ 紙吹雪の初期設定パラメータ
    // =========================================================
    int   confettiNum = 120;     // 降らせる紙吹雪の総枚数

    // 落下速度（少し早めに設定しました）
    float fallSpeedMin = 1.0f;    // 落下速度の最小値
    float fallSpeedMax = 5.0f;    // 落下速度の最大値

    // サイズ（中心からの半径）
    float sizeMin = 3.0f;    // サイズの最小値
    float sizeMax = 8.0f;    // サイズの最大値

    // 回転
    float rotSpeedMax = 0.15f;   // 回転スピードの最大値

    // 配置エリア
    float spawnAreaWidth = 250.0f;  // 画面端から配置する幅（左右それぞれ250px）
    float spawnHeight = 800.0f;  // 初期配置時の縦のばらつき範囲
    // =========================================================

    // 華やかな色の候補を用意
    unsigned int colors[] = {
        GetColor(255, 100, 100), // 赤系
        GetColor(100, 255, 100), // 緑系
        GetColor(100, 150, 255), // 青系
        GetColor(255, 230,  50), // 黄系
        GetColor(255, 150, 255), // ピンク系
        GetColor(255, 150,  50)  // オレンジ系
    };

    for (int i = 0; i < confettiNum; ++i) {
        Confetti c;
        // 画面幅1000の「左側」か「右側」に配置する
        bool isLeft = (i % 2 == 0);
        float randX = (float)GetRand(1000) / 1000.0f * spawnAreaWidth;
        c.baseX = isLeft ? randX : (1000.0f - spawnAreaWidth) + randX;

        c.x = c.baseX;
        c.y = -((float)GetRand(1000) / 1000.0f * spawnHeight); // 初期は画面外(上)にランダム配置

        // 速度やサイズ、回転速度を変数を使ってランダムに決定
        float randSpeed = (float)GetRand(1000) / 1000.0f;
        c.vy = fallSpeedMin + randSpeed * (fallSpeedMax - fallSpeedMin);

        float randSize = (float)GetRand(1000) / 1000.0f;
        c.size = sizeMin + randSize * (sizeMax - sizeMin);

        c.angle = (float)GetRand(314) / 100.0f;

        float randRot = (float)GetRand(1000) / 1000.0f;
        c.angleSpeed = -rotSpeedMax + randRot * (rotSpeedMax * 2.0f);

        c.color = colors[GetRand(5)]; // 色をランダムに決定

        m_confettis.push_back(c);
    }
}

// 紙吹雪の動きを計算するメソッド
void ResultScene::UpdateConfetti() {
    // =========================================================
    // ▼ 紙吹雪の動きパラメータ
    // =========================================================
    float swingAmp = 25.0f;   // 左右にヒラヒラ揺れる幅（大きくすると激しく左右に動く）
    float swingFreq = 30.0f;   // 揺れの周期（小さくすると小刻みに揺れる）
    float screenBottom = 800.0f;  // 画面の下端座標
    // =========================================================

    for (auto& c : m_confettis) {
        c.y += c.vy; // 下へ落下

        // Y座標とサイン波(sin)を組み合わせることで、左右にヒラヒラと揺れながら落ちる
        c.x = c.baseX + std::sin(c.y / swingFreq) * swingAmp;

        c.angle += c.angleSpeed; // くるくる回す

        // 画面の下端を越えたら、再び上空に戻す
        if (c.y > screenBottom + c.size) {
            c.y = -c.size - (float)GetRand(100);
        }
    }
}

// 紙吹雪を描画するメソッド
void ResultScene::DrawConfetti() const {
    for (const auto& c : m_confettis) {
        // すでに自作されている回転描画メソッドを流用！
        DrawRotatedBox(c.x, c.y, 0.0f, 0.0f, c.size, c.angle, c.color, TRUE);
    }
}

SceneName ResultScene::Update() {
    // ----------------------------------------------------
    // 回転アニメーションの更新
    // ----------------------------------------------------
    m_timer++;

    // 1サイクルの合計フレーム数
    int cycleFrames = m_rotateFrames + m_stopFrames;

    if (m_timer >= cycleFrames) {
        m_timer = 0; // タイマーをリセット
        m_rotateStep = (m_rotateStep + 1) % 4;
    }

    const float pi = 3.14159265f;
    // 正回転の基準角度
    float baseAngle = m_rotateStep * (pi / 2.0f);
    // 逆回転の基準角度（マイナス方向）
    float baseAngleRev = m_rotateStep * (-pi / 2.0f);

    if (m_timer < m_rotateFrames) {
        // --- 回転フェーズ ---
        float progress = static_cast<float>(m_timer) / static_cast<float>(m_rotateFrames);

        // 滑らかに回って止まる計算 (イージング)
        float ease = (1.0f - std::cos(progress * pi)) / 2.0f;

        // 正回転と逆回転をそれぞれ計算
        m_angle = baseAngle + (pi / 2.0f) * ease;
        m_angleRev = baseAngleRev - (pi / 2.0f) * ease;
    }
    else {
        // --- 停止フェーズ ---
        m_angle = baseAngle + (pi / 2.0f);
        m_angleRev = baseAngleRev - (pi / 2.0f);
    }

    // ----------------------------------------------------
       // 物理演算の共通設定
       // ----------------------------------------------------
    float gravity = 0.8f;
    float limitPos = outerHalf - innerHalf;
    float bounciness = 0.5f;

    // ----------------------------------------------------
    // ① 正回転する外枠に対しての処理
    // ----------------------------------------------------
    float localGravityX = std::sin(m_angle) * gravity;
    float localGravityY = std::cos(m_angle) * gravity;

    m_innerVX += localGravityX;
    m_innerVY += localGravityY;
    m_innerX += m_innerVX;
    m_innerY += m_innerVY;

    if (m_innerX > limitPos) { m_innerX = limitPos; m_innerVX *= -bounciness; }
    else if (m_innerX < -limitPos) { m_innerX = -limitPos; m_innerVX *= -bounciness; }

    if (m_innerY > limitPos) { m_innerY = limitPos; m_innerVY *= -bounciness; }
    else if (m_innerY < -limitPos) { m_innerY = -limitPos; m_innerVY *= -bounciness; }

    // ----------------------------------------------------
    // ② ★追加：逆回転する外枠に対しての処理
    // ----------------------------------------------------
    // 逆回転の角度(m_angleRev)を使って重力の方向を計算
    float localGravityRevX = std::sin(m_angleRev) * gravity;
    float localGravityRevY = std::cos(m_angleRev) * gravity;

    m_innerRevVX += localGravityRevX;
    m_innerRevVY += localGravityRevY;
    m_innerRevX += m_innerRevVX;
    m_innerRevY += m_innerRevVY;

    if (m_innerRevX > limitPos) { m_innerRevX = limitPos; m_innerRevVX *= -bounciness; }
    else if (m_innerRevX < -limitPos) { m_innerRevX = -limitPos; m_innerRevVX *= -bounciness; }

    if (m_innerRevY > limitPos) { m_innerRevY = limitPos; m_innerRevVY *= -bounciness; }
    else if (m_innerRevY < -limitPos) { m_innerRevY = -limitPos; m_innerRevVY *= -bounciness; }

    // ----------------------------------------------------

    // 紙吹雪の座標を更新
    UpdateConfetti();

    // Enterキーでタイトル画面に移行
	if (CheckHitKey(KEY_INPUT_RETURN)) {
		return SceneName::SELECT;
	}

	return SceneName::RESULT;
}

// --- 新しく追加する回転描画用の共通メソッド ---
void ResultScene::DrawRotatedBox(float centerX, float centerY, float offsetX, float offsetY, float halfSize, float angle, unsigned int color, int fillFlag) const {
    // 共通の回転計算用のサイン・コサイン
    float s = std::sin(angle);
    float c = std::cos(angle);

    // 中心からのオフセット（ズレ）を加味したローカル座標の4頂点
    float localX[4] = { offsetX - halfSize, offsetX + halfSize, offsetX + halfSize, offsetX - halfSize };
    float localY[4] = { offsetY - halfSize, offsetY - halfSize, offsetY + halfSize, offsetY + halfSize };

    int outX[4], outY[4];
    for (int i = 0; i < 4; ++i) {
        // 回転行列を適用して画面の座標（ワールド座標）に変換
        outX[i] = static_cast<int>(centerX + localX[i] * c - localY[i] * s);
        outY[i] = static_cast<int>(centerY + localX[i] * s + localY[i] * c);
    }

    // 描画
    DrawQuadrangle(
        outX[0], outY[0], outX[1], outY[1],
        outX[2], outY[2], outX[3], outY[3],
        color, fillFlag
    );
}

void ResultScene::Draw() const {
    // 紙吹雪を描画する（文字やメインの四角形の「奥」に描画される）
    DrawConfetti();

    // ゲームクリア文字の描画（横中央揃え）
    const TCHAR* clearStr = _T("STAGE CLEAR!");
    int clearWidth = GetDrawFormatStringWidthToHandle(Font.GetBig(), clearStr);
    int clearX = (WIN_MAX_X - clearWidth) / 2;
    int clearY = 100;

    DrawFormatStringToHandle(clearX, clearY, Col.GetYel(), Font.GetBig(), clearStr);


    // Enterキー誘導テキストの描画（横中央揃え）
    const TCHAR* resultStr = _T("Enterキーでステージ選択画面に戻る");
    int resultWidth = GetDrawFormatStringWidthToHandle(Font.GetNormal(), resultStr);
    int resultX = (WIN_MAX_X - resultWidth) / 2;
    int resultY = 400;

    DrawFormatStringToHandle(resultX, resultY, Col.GetWhi(), Font.GetNormal(), resultStr);

    // --- ここから回転する四角形の描画処理 ---
    float centerX = 830.0f;
    float centerY = 630.0f;
    float centerX2 = 230.0f;

    // --- 左側：逆回転する四角形 ---
    DrawRotatedBox(centerX2, centerY, 0.0f, 0.0f, outerHalf, m_angleRev, Col.GetWhi(), FALSE);
    // 逆回転用の座標(m_innerRevX, Y)と、逆回転用の角度(m_angleRev)を使う
    DrawRotatedBox(centerX2, centerY, m_innerRevX, m_innerRevY, innerHalf, m_angleRev, Col.GetRed(), TRUE);

    // --- 右側：正回転する四角形 ---
    DrawRotatedBox(centerX, centerY, 0.0f, 0.0f, outerHalf, m_angle, Col.GetWhi(), FALSE);
    // 正回転用の座標と角度を使う
    DrawRotatedBox(centerX, centerY, m_innerX, m_innerY, innerHalf, m_angle, Col.GetRed(), TRUE);
}