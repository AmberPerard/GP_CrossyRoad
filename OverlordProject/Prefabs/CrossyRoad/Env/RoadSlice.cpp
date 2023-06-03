#include "stdafx.h"
#include "RoadSlice.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow.h"

RoadSlice::RoadSlice(int amountOfObstacles, int maxWidth, UINT textureID)
	: TerrainSlice(amountOfObstacles, maxWidth, textureID), m_CarSpeed(2.f), m_CarSpawnTimer(0)
{
	//choose the direction of the cars
	switch (rand() % 2)
	{
	case 0:
		m_CarDirection = int(carDirection::LEFT);
		break;
	case 1:
		m_CarDirection = int(carDirection::RIGHT);
		break;
	default:
		m_CarDirection = int(carDirection::LEFT);
		break;
	}
	//get random spawn interval
	m_CarSpawnInterval = 0;
}

RoadSlice::~RoadSlice()
{
}

bool RoadSlice::IsSlicePassable(int /*xPos*/)
{
	return true;
}

void RoadSlice::Initialize(const SceneContext&)
{
	m_pSlice = new GameObject();
	auto model = m_pSlice->AddComponent(new ModelComponent(L"../Resources/Meshes/CrossyRoad/env/slice.ovm"));
	auto mat = dynamic_cast<DiffuseMaterial_Shadow*>(MaterialManager::Get()->GetMaterial(m_TextureID));
	model->SetMaterial(mat);
	AddChild(m_pSlice);
}

void RoadSlice::Update(const SceneContext& sceneContext)
{
	m_CarSpawnTimer += sceneContext.pGameTime->GetElapsed();

	if(m_CarSpawnTimer >= m_CarSpawnInterval)
	{
		m_CarSpawnTimer = 0.f;
		SetCarSpawnInterval();
		//SpawnCar();
	}

	deleteCars();
}

void RoadSlice::SetCarSpawnInterval()
{
	//get random spawn interval
	m_CarSpawnInterval = float((rand() % int(m_CarSpawnMaxTime + m_CarSpawnMinTime)) + m_CarSpawnMinTime);
}

void RoadSlice::SpawnCar()
{
	//Spawn a car at a full left or full right position -> depending on left or right street
	float spawnPos = m_MaxWidth * -1.5f * m_CarDirection;

	Car* pCar = new Car(m_CarSpeed, m_CarDirection);
	m_pCars.emplace_back(pCar);

	pCar->GetTransform()->Translate(spawnPos, 0.4f, 0.f);
	AddChild(pCar);

}

void RoadSlice::deleteCars()
{
	for (auto car : m_pCars)
	{
		if(car->GetTransform()->GetPosition().x * m_CarDirection > (m_MaxWidth * 2.f))
		{
			m_pCars.erase(std::remove(m_pCars.begin(), m_pCars.end(), car));
			RemoveChild(car,true);
		}
	}
}
