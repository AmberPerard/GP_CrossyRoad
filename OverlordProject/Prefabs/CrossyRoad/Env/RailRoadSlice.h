#pragma once
#include "TerrainSlice.h"
#include "Prefabs/CrossyRoad/Vehicle/Train.h"

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
protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
private:
	int m_TrainDirection;
	float m_TrainSpeed{30.f};

	float m_TrainSpawnInterval{ 0 };
	float m_TrainSpawnMaxTime{ 4.f };
	float m_TrainSpawnMinTime{ 2.f };
	void SetTrainSpawnInterval();

	float m_TrainSpawnTimer;

	std::vector<Train*> m_pTrains;
	const int m_NrCarriages = 6;
	const float m_CarriageLength = 4.6f;
	float m_SpawnPos;
	void SpawnTrain();
	void deleteTrains();
};
