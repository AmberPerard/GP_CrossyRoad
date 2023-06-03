#pragma once
#include "CrossyCharacter.h"

class TerrainSlice;

enum TerrainType
{
	GRASS,
	WATER,
	ROAD,
	RAILROAD
};

class TerrainGenerator final : public GameObject
{
public:
	TerrainGenerator(CrossyCharacter* trackedCharacter, int slicesAhead, int width);
	~TerrainGenerator() override;

	TerrainGenerator(const TerrainGenerator& other) = delete;
	TerrainGenerator(TerrainGenerator&& other) noexcept = delete;
	TerrainGenerator& operator=(const TerrainGenerator& other) = delete;
	TerrainGenerator& operator=(TerrainGenerator&& other) noexcept = delete;

	//custom functions
	void Reset();
	TerrainSlice* GetCurrentTerrain(int sliceNr);
	TerrainSlice* GetNextTerrain();
	TerrainSlice* GetPreviousTerrain();
	bool IsCurrentSlicePassable(int xPos, int sliceNr);

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& sceneContext) override;

private:
	std::map<int, TerrainSlice*> m_TerrainSlices;

	TerrainSlice* m_CurrentSlice = nullptr;
	TerrainType m_PreviousTerrainType;

	CrossyCharacter* m_TrackedCharacter = nullptr;

	int m_MaxWidth;

	//Terrain Generation variables
	int m_CurrentSliceNumber{};
	int m_SlicesAhead{};
	int m_NrBlankGrassSlices{};
	const int m_MaxBlankSlices = 5;

	//Map for the weights of the terrain types
	std::map<TerrainType, int> m_TerrainWeights{};
	int m_TotalWeight;

	static UINT m_GrassTextureID;
	static UINT m_WaterTextureID;
	static UINT m_RoadTextureID;
	static UINT m_RailroadTextureID;



	//Function to spawn a new terrain slice
	void SpawnNewSlice();
};

