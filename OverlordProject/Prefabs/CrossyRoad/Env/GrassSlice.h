#pragma once
#include "TerrainSlice.h"

class GrassSlice final : public TerrainSlice
{
public:
	GrassSlice(int amountOfObstacles, int maxWidth, UINT textureID);
	~GrassSlice() override;

	GrassSlice(const GrassSlice& other) = delete;
	GrassSlice(GrassSlice&& other) noexcept = delete;
	GrassSlice& operator=(const GrassSlice& other) = delete;
	GrassSlice& operator=(GrassSlice&& other) noexcept = delete;

	//Custom functions
	virtual bool IsSlicePassable(int xPos) override;
	void Initialize(const SceneContext&) override;
private:
	void CreateSideTrees();
	void CreateObstacles();
};

