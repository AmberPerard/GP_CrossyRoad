#pragma once
#include "TerrainSlice.h"

class RoadSlice final : public TerrainSlice
{
public:
	RoadSlice(int amountOfObstacles, int maxWidth, UINT textureID);
	~RoadSlice() override;

	RoadSlice(const RoadSlice& other) = delete;
	RoadSlice(RoadSlice&& other) noexcept = delete;
	RoadSlice& operator=(const RoadSlice& other) = delete;
	RoadSlice& operator=(RoadSlice&& other) noexcept = delete;

	//Custom functions
	virtual bool IsSlicePassable(int xPos) override;
	void Initialize(const SceneContext&) override;
};


