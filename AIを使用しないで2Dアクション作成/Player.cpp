#include "Player.h"
#include "GameData.h"

Player::Player() {
	x = 100.0f;
	y = 100.0f;
	velocityX = 0;
	velocityY = 0;
	speed = 5.0f;
}

Player::~Player() {}

void Player::Update() {

}

void Player::Draw() const{
	DrawBox(x, y, x + PLAYER_SIZE, y + PLAYER_SIZE, Col.GetWhi(), TRUE);
}

