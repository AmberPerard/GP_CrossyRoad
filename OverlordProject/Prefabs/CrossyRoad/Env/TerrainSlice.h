#pragma once

class TerrainSlice : public GameObject
{
public:
	TerrainSlice(int amountOfObstacles, int maxWidth, UINT textureID);
	~TerrainSlice() override;

	TerrainSlice(const TerrainSlice& other) = delete;
	TerrainSlice(TerrainSlice&& other) noexcept = delete;
	TerrainSlice& operator=(const TerrainSlice& other) = delete;
	TerrainSlice& operator=(TerrainSlice&& other) noexcept = delete;

	//Custom functions
	virtual bool IsSlicePassable(int xPos) = 0;
	/*TerrainType m_TerrainType{TerrainType::GRASS};*/
public:
	void Initialize(const SceneContext&) override = 0;

	//Map for the obstacles and the position on the x axis
	GameObject* m_pSlice;
	std::map<int, GameObject*> m_Obstacles{};
	int m_NrObstacles{};
	int m_MaxWidth{};
	UINT m_TextureID{};
};
