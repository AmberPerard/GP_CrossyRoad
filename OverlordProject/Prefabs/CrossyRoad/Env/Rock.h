#pragma once
#include "Obstacle.h"

class Rock : public Obstacle
{
	Rock(int amountOfObstacles, int maxWidth, UINT textureID);
	~Rock() override;

	Rock(const Rock& other) = delete;
	Rock(Rock&& other) noexcept = delete;
	Rock& operator=(const Rock& other) = delete;
	Rock& operator=(Rock&& other) noexcept = delete;
};

