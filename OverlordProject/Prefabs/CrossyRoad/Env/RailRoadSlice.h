#pragma once
#include "TerrainSlice.h"

class RailRoadSlice final : public TerrainSlice
{
public:
	RailRoadSlice(int amountOfObstacles, int maxWidth, UINT textureID);
	~RailRoadSlice() override;

	RailRoadSlice(const RailRoadSlice& other) = delete;
	RailRoadSlice(RailRoadSlice&& other) noexcept = delete;
	RailRoadSlice& operator=(const RailRoadSlice& other) = delete;
	RailRoadSlice& operator=(RailRoadSlice&& other) noexcept = delete;

	//Custom functions
	virtual bool IsSlicePassable(int xPos) override;
	void Initialize(const SceneContext&) override;
};
