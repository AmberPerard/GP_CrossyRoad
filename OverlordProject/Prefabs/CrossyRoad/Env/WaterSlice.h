#pragma once
#include "TerrainSlice.h"

class WaterSlice final : public TerrainSlice
{
public:
	WaterSlice(int amountOfLilyPads, int maxWidth, UINT textureID);
	~WaterSlice() override;

	WaterSlice(const WaterSlice& other) = delete;
	WaterSlice(WaterSlice&& other) noexcept = delete;
	WaterSlice& operator=(const WaterSlice& other) = delete;
	WaterSlice& operator=(WaterSlice&& other) noexcept = delete;

	//Custom functions
	virtual bool IsSlicePassable(int xPos) override;
	bool IsLilyPad(int xPos);
	void Initialize(const SceneContext&) override;
private:
	std::map<int, GameObject*> m_pLilyPads;
};

