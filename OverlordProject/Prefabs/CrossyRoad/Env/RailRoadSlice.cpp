#include "stdafx.h"
#include "RailRoadSlice.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow.h"

RailRoadSlice::RailRoadSlice(int amountOfObstacles, int maxWidth, UINT textureID)
	:TerrainSlice(amountOfObstacles, maxWidth,textureID)
{
	//choose the direction of the cars
	switch (rand() % 2)
	{
	case 0:
		m_TrainDirection = -1;
		break;
	case 1:
		m_TrainDirection = 1;
		break;
	default:
		m_TrainDirection = -1;
		break;
	}
	//SetTrainSpawnInterval();
	m_TrainSpawnInterval = 0;
	//Spawn a Train at a full left or full right position -> depending on left or right street
	m_SpawnPos = m_MaxWidth * -1.5f * m_TrainDirection;
}

RailRoadSlice::~RailRoadSlice()
{
}

bool RailRoadSlice::IsSlicePassable(int /*xPos*/)
{
	return true;
}

void RailRoadSlice::Initialize(const SceneContext&)
{
	m_pSlice = new GameObject();
	auto model = m_pSlice->AddComponent(new ModelComponent(L"../Resources/Meshes/CrossyRoad/env/railroad.ovm"));
	DiffuseMaterial_Shadow* mat = dynamic_cast<DiffuseMaterial_Shadow*>(MaterialManager::Get()->GetMaterial(m_TextureID));

	model->SetMaterial(mat);
	AddChild(m_pSlice);
}

void RailRoadSlice::Update(const SceneContext& sceneContext)
{
	m_TrainSpawnTimer += sceneContext.pGameTime->GetElapsed();

	if (m_TrainSpawnTimer >= m_TrainSpawnInterval)
	{
		m_TrainSpawnTimer = 0.f;
		SetTrainSpawnInterval();
		SpawnTrain();
	}

	deleteTrains();
}

void RailRoadSlice::SetTrainSpawnInterval()
{
	//get random spawn interval
	m_TrainSpawnInterval = float((rand() % int(m_TrainSpawnMaxTime + m_TrainSpawnMinTime)) + m_TrainSpawnMinTime);
}

void RailRoadSlice::SpawnTrain()
{

	m_pTrains.emplace_back(AddChild(new Train(m_TrainSpeed, m_TrainDirection, m_SpawnPos - (m_TrainDirection * m_CarriageLength * 0), Train::TrainPiece::front)));

	for (int i{1}; i < m_NrCarriages-1; ++i)
	{
		m_pTrains.emplace_back(AddChild(new Train(m_TrainSpeed, m_TrainDirection, m_SpawnPos - (m_TrainDirection * m_CarriageLength * i), Train::TrainPiece::middle)));
	}

	m_pTrains.emplace_back(AddChild(new Train(m_TrainSpeed, m_TrainDirection, m_SpawnPos - (m_TrainDirection * m_CarriageLength * (m_NrCarriages-1)), Train::TrainPiece::end)));
}

void RailRoadSlice::deleteTrains()
{
	for (auto Train : m_pTrains)
	{
		if (Train->GetTransform()->GetPosition().x * m_TrainDirection > (m_MaxWidth * 2.f))
		{
			m_pTrains.erase(std::remove(m_pTrains.begin(), m_pTrains.end(), Train));
			RemoveChild(Train, true);
		}
	}
}
