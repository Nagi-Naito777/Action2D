// プレイヤー操作クラス
#pragma once
#include "DxLib.h"
#include "Player.h"
class PlayerController
{
private:
	// 操作対象のプレイヤー
	Player* targetplayer;
public:
	PlayerController(Player* p);

	// 更新処理
	void Update();

};