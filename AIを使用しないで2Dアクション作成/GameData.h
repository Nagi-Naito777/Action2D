// ゲーム内で共通して使うデータなどをまとめるヘッダー
#pragma once

//画面サイズ指定マクロ
#define WIN_MAX_X 1000
#define WIN_MAX_Y 800

// プレイヤーサイズ
#define PLAYER_SIZE 50

// ブロックサイズ
#define BLOCK_SIZE 50

#include "DxLib.h"

// フォント管理クラス
class FontManager
{
private:
    int smallHandle;
    int normalHandle;
    int bigHandle;

public:
    // コンストラクタ
    FontManager() : smallHandle(-1), normalHandle(-1), bigHandle(-1) {}
    ~FontManager() = default;

    // フォント初期化
    void Init() {
        smallHandle = CreateFontToHandle(_T("MS ゴシック"), 16, 3);
        normalHandle = CreateFontToHandle(_T("MS ゴシック"), 32, 3);
        bigHandle = CreateFontToHandle(_T("MS ゴシック"), 64, 5);
    }

    // フォントデータ削除
    void End() {
        DeleteFontToHandle(smallHandle);
        DeleteFontToHandle(normalHandle);
        DeleteFontToHandle(bigHandle);
    }

    // ゲッター関数
    int GetSmall() const { return smallHandle; }
    int GetNormal() const { return normalHandle; }
    int GetBig() const { return bigHandle; }
};

class ColorManager {
private:
    int m_red;    // 赤
    int m_blu;    // 青
    int m_gre;    // 緑
    int m_yel;    // 黄色
    int m_bla;    // 黒
    int m_whi;    // 白
    int m_gra;    // 灰色
    int m_sky;    // 水色
public:
    // コンストラクタ
    ColorManager()
        : m_red(0)
        , m_blu(0)
        , m_gre(0)
        , m_yel(0)
        , m_bla(0)
        , m_whi(0)
        , m_gra(0)
        , m_sky(0)
    {}

    // 初期化処理
    void Init() {
        m_red = GetColor(255, 0, 0);    // 赤
        m_blu = GetColor(0, 0, 255);    // 青
        m_gre = GetColor(0, 255, 0);    // 緑
        m_yel = GetColor(255, 255, 0);  // 黄色
        m_bla = GetColor(0, 0, 0);      // 黒
        m_whi = GetColor(255, 255, 255);// 白
        m_gra = GetColor(128, 128, 128);// 灰色
        m_sky = GetColor(0, 255, 255);
    }

    // ゲッター関数
    int GetRed() const { return m_red; }
    int GetBlu() const { return m_blu; }
    int GetGre() const { return m_gre; }
    int GetYel() const { return m_yel; }
    int GetBla() const { return m_bla; }
    int GetWhi() const { return m_whi; }
    int GetGra() const { return m_gra; }
    int GetSky() const { return m_sky; }
};

extern ColorManager Col;
extern FontManager Font;