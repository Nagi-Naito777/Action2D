#include "GameData.h"
#include "DxLib.h"

void GetRotatedPosition(float centerX, float centerY, float x, float y,
    float* outX, float* outY, float angleDeg) {
    // 角度を度数(Deg)からラジアン(Rad)に変更
    float rad = angleDeg * DX_PI_F / 180.0f;
    float cosA = std::cos(rad);
    float sinA = std::sin(rad);

    // 中心からの相対位置
    float dx = x - centerX;
    float dy = y - centerY;

    // 回転後の相対位置を計算
    float rx = dx * cosA - dy * sinA;
    float ry = dx * sinA + dy * cosA;

    // 中心を足して絶対座標に戻す
    *outX = rx + centerX;
    *outY = ry + centerY;
}

GravityDir GravityManager::currentDir = GravityDir::Down;

// 回転ロジック
void GravityManager::Rotate() {
    switch (GravityManager::currentDir)
    {
    case GravityDir::Down:
        GravityManager::currentDir = GravityDir::Left;
        break;
    case GravityDir::Left:
        GravityManager::currentDir = GravityDir::Up;
        break;
    case GravityDir::Up:
        GravityManager::currentDir = GravityDir::Right;
        break;
    case GravityDir::Right:
        GravityManager::currentDir = GravityDir::Down;
        break;
    default:
        break;
    }
}

// --- BGM・SE関係の関数処理 ---

// コンストラクタ
SoundManager::SoundManager() {}
SoundManager::~SoundManager() = default;

// 初期化(BGM・SEの読み込み)
void SoundManager::Init() {
    // ==========================================
    // BGMの読み込み
    // ==========================================

    // タイトル、ステージ選択、リザルト画面で共通して使うBGM
    m_sounds["BGM_MAIN"] = LoadSoundMem("data/BGM/タイトルBGM.mp3");

    // ゲームプレイ画面で使うBGM
    m_sounds["BGM_PLAY"] = LoadSoundMem("data/BGM/ステージ.mp3");


    // ==========================================
    // 効果音(SE)の読み込み
    // ==========================================

    // カーソル音や決定時のシステム音
    m_sounds["SE_SYSTEM"] = LoadSoundMem("data/SE/カーソル.wav");

    // アクション・ギミック用の音
    m_sounds["SE_ROTATE_START"] = LoadSoundMem("data/SE/回転開始.wav");
    m_sounds["SE_ROTATE"] = LoadSoundMem("data/SE/回転中.wav");
    m_sounds["SE_CLEAR"] = LoadSoundMem("data/SE/クリア.wav"); // クリア時の音など
}

// 音声データの全削除（ゲーム終了時に呼ぶ）
void SoundManager::End() {
    for (auto& pair : m_sounds) {
        DeleteSoundMem(pair.second);
    }
    m_sounds.clear();
}

// 再生関数（ご要望の分割処理）
// isLoop: trueならBGM(ループ)、falseならSE(単音)
// fromTop: trueなら最初から再生、falseなら途中から
void SoundManager::Play(const std::string& key, bool isLoop, bool fromTop) {
    auto it = m_sounds.find(key);
    if (it == m_sounds.end()) return; // 読み込まれていない場合は無視

    int handle = it->second;

    if (isLoop) {
        // --- BGM（ループ再生）の処理 ---
        // ※既に再生中かつ頭出し不要なら、再リセットしないような配慮
        if (CheckSoundMem(handle) == 1 && !fromTop) {
            return;
        }
        PlaySoundMem(handle, DX_PLAYTYPE_LOOP, fromTop ? TRUE : FALSE);
    }
    else {
        // --- 効果音（単音・バックグラウンド再生）の処理 ---
        PlaySoundMem(handle, DX_PLAYTYPE_BACK, TRUE);
    }
}

// 指定した音声を停止する関数
void SoundManager::Stop(const std::string& key) {
    auto it = m_sounds.find(key);
    if (it != m_sounds.end()) {
        StopSoundMem(it->second);
    }
}

// 全ての音声を停止する関数（シーン遷移時などに便利）
void SoundManager::StopAll() {
    for (auto& pair : m_sounds) {
        StopSoundMem(pair.second);
    }
}

// 音量変更関数 (volume: 0 〜 255)
void SoundManager::SetVolume(const std::string& key, int volume) {
    auto it = m_sounds.find(key);
    if (it != m_sounds.end()) {
        ChangeVolumeSoundMem(volume, it->second);
    }
}