#pragma once
#include "Game.h"

class TronGame final : public Game
{
public:
	TronGame(unsigned int width, unsigned int height);
	~TronGame() override = default;
	TronGame(const TronGame& other) = delete;
	TronGame(TronGame&& other) = delete;
	TronGame& operator=(const TronGame& other) = delete;
	TronGame& operator=(TronGame&& other) = delete;


	void LoadGame() override;
};

