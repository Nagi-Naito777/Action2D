#pragma once
#include <string>
#include <functional>

// 許可するアクションのビットフラグ(チュートリアルのキーロック)
enum ActionFlag {
	ACTION_NONE = 0,
	ACTION_MOVE_RIGHT = 1 << 0,  // 0000 0001 : 右に移動(Dキーや→キーなど)
	ACTION_MOVE_LEFT = 1 << 1,  // 0000 0010 : 左に移動
	ACTION_ROTATE_R90 = 1 << 2,  // 0000 0100 : 右に90°回転
	ACTION_ROTATE_L90 = 1 << 3,  // 0000 1000 : 左に90°回転
	ACTION_ROTATE_180 = 1 << 4,  // 0001 0000 : 180°回転
	ACTION_RESET = 1 << 5,  // 0010 0000 : ステージリセット
	ACTION_ALL = 0xFFFFFFFF // 全許可
};

// チュートリアルの1フェーズ(手順)を管理する構造体
struct TutorialTask {
	std::string message;               // 画面に表示する説明文
	unsigned int allowedActions;       // この手順で許可する操作
	std::function<bool()> checkClear;  // この手順をクリアしたか判定する関数
};
