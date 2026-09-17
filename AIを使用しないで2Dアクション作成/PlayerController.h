// プレイヤー操作クラス
#pragma once
#include "DxLib.h"
#include "Player.h"
#include "Tutorial.h"

class PlayerController
{
private:
	// 操作対象のプレイヤー
	Player* targetplayer;

	// 許可されているアクションのフラグ（初期状態は全許可）
	unsigned int allowedActions = ACTION_ALL;
public:
	PlayerController(Player* p);
	~PlayerController();

	// 更新処理
	void Update();

	// 【追加】外部(チュートリアルシーンなど)から許可アクションを設定する
	void SetAllowedActions(unsigned int actions) {
		allowedActions = actions;
	}
};