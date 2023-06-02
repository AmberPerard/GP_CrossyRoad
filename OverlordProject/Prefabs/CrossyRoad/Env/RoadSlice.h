#pragma once
#include "TerrainSlice.h"
#include "Prefabs/CrossyRoad/Vehicle/Car.h"

enum class carDirection
{
	LEFT = -1,
	RIGHT = 1
};


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

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
private:
	int m_CarDirection;
	float m_CarSpeed;

	float m_CarSpawnInterval;
	float m_CarSpawnMaxTime{4.f};
	float m_CarSpawnMinTime{2.f};
	void SetCarSpawnInterval();

	float m_CarSpawnTimer;

	std::vector<Car*> m_pCars;
	void SpawnCar();
	void deleteCars();
};


