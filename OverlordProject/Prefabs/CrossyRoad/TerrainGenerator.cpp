#include "stdafx.h"
#include "TerrainGenerator.h"
#include "Env/TerrainSlice.h"
#include "Env/GrassSlice.h"
#include "Env/RailRoadSlice.h"
#include "Env/RoadSlice.h"
#include "Env/WaterSlice.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"

UINT TerrainGenerator::m_GrassTextureID = 0;
UINT TerrainGenerator::m_WaterTextureID = 0;
UINT TerrainGenerator::m_RailroadTextureID = 0;
UINT TerrainGenerator::m_RoadTextureID = 0;

TerrainGenerator::TerrainGenerator(CrossyCharacter* trackedCharacter, int slicesAhead, int width)
	: m_TrackedCharacter(trackedCharacter)
	, m_SlicesAhead(slicesAhead)
	, m_MaxWidth(width)
{
	m_TerrainWeights.insert({ TerrainType::GRASS, 10 });
	m_TerrainWeights.insert({ TerrainType::ROAD, 6 });
	m_TerrainWeights.insert({ TerrainType::WATER, 2 });
	m_TerrainWeights.insert({ TerrainType::RAILROAD, 1 });

	for (auto pair : m_TerrainWeights)
	{
		m_TotalWeight += pair.second;
	}

	m_NrBlankGrassSlices = m_MaxBlankSlices;
}

TerrainGenerator::~TerrainGenerator()
{
	m_TerrainSlices.clear();
}

void TerrainGenerator::Reset()
{
	for (auto& pair : m_TerrainSlices)
	{
		RemoveChild(pair.second, true);
	}
	m_TerrainSlices.clear();

	m_CurrentSlice = nullptr;
	m_CurrentSliceNumber = 0;
	//put a few grass slices down by default for the player to start on
		//first 5 slices before slice 0
	TerrainSlice* slice = nullptr;
	for (int i{ -5 }; i < 0; ++i)
	{
		auto grass = new GrassSlice(100, m_MaxWidth, m_GrassTextureID);
		slice = AddChild(grass);
		m_TerrainSlices.insert(std::pair{ i, slice });
		slice->GetTransform()->Translate(0.f, -0.4f, float(i));
		//slice->GetTransform()->Scale(10.f);
	}

	m_NrBlankGrassSlices = m_MaxBlankSlices;
}

TerrainSlice* TerrainGenerator::GetCurrentTerrain(int sliceNr)
{
	auto currentSlice = m_TerrainSlices.find(sliceNr);
	if (currentSlice != m_TerrainSlices.end())
	{
		return currentSlice->second;
	}
	return nullptr;
}

TerrainSlice* TerrainGenerator::GetNextTerrain()
{
	return GetCurrentTerrain(m_CurrentSliceNumber + 1);
}

TerrainSlice* TerrainGenerator::GetPreviousTerrain()
{
	return GetCurrentTerrain(m_CurrentSliceNumber - 1);
}

bool TerrainGenerator::IsCurrentSlicePassable(int xPos, int sliceNr)
{
	TerrainSlice* pSlice = m_TerrainSlices.find(sliceNr)->second;
	if (pSlice)
	{
		return pSlice->IsSlicePassable(xPos);
	}
	else
	{
		return false;
	}
}

void TerrainGenerator::Initialize(const SceneContext& /*sceneContext*/)
{
	if (m_GrassTextureID == 0)
	{
		auto material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
		//Initializing textures for the terrain
		material->SetDiffuseTexture(L"../Resources/Textures/crossy/env/light-grass.png");
		m_GrassTextureID = material->GetMaterialId();
	}

	if (m_WaterTextureID == 0)
	{
		auto material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
		material->SetDiffuseTexture(L"../Resources/Textures/crossy/env/water.png");
		m_WaterTextureID = material->GetMaterialId();
	}

	if (m_RoadTextureID == 0)
	{
		auto material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
		material->SetDiffuseTexture(L"../Resources/Textures/crossy/env/road.png");
		m_RoadTextureID = material->GetMaterialId();
	}

	if (m_RailroadTextureID == 0)
	{
		auto material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
		material->SetDiffuseTexture(L"../Resources/Textures/crossy/env/railroad.png");
		m_RailroadTextureID = material->GetMaterialId();
	}


	//put a few grass slices down by default for the player to start on
	//first 5 slices before slice 0
	TerrainSlice* slice = nullptr;
	for (int i{ -5 }; i < 0; ++i)
	{
		auto grass = new GrassSlice(100, m_MaxWidth, m_GrassTextureID);
		slice = AddChild(grass);
		m_TerrainSlices.insert(std::pair{ i, slice });
		slice->GetTransform()->Translate(0.f, -0.4f, float(i));
		//slice->GetTransform()->Scale(10.f);
	}

	m_NrBlankGrassSlices = m_MaxBlankSlices;

	while (m_CurrentSliceNumber <= m_SlicesAhead)
	{
		SpawnNewSlice();
	}
	std::cout << "TerrainGenerator initialized" << std::endl;
}

void TerrainGenerator::Update(const SceneContext& /*sceneContext*/)
{
	//update the terrain, spawn new slices when going forward, and despawn old slices when they go out of screen
	if (m_TrackedCharacter)
	{
		while (static_cast<int>(round(m_TrackedCharacter->GetTransform()->GetPosition().z / 10)) + m_SlicesAhead >= m_CurrentSliceNumber)
		{
			SpawnNewSlice();
		}

		if (!m_TrackedCharacter->IsDead())
		{
			//Despawn old slices
			auto slice = m_TerrainSlices.find(((int)m_TrackedCharacter->GetTransform()->GetPosition().z / 10 - m_SlicesAhead));
			if (slice != m_TerrainSlices.end())
			{
				RemoveChild(slice->second, true);
				m_TerrainSlices.erase(slice);
			}
		}

	}
}

void TerrainGenerator::SpawnNewSlice()
{
	TerrainSlice* slice = nullptr;
	if (m_NrBlankGrassSlices > 0)
	{
		slice = new GrassSlice(0, m_MaxWidth, m_GrassTextureID);
		AddChild(slice);
		--m_NrBlankGrassSlices;

	}
	else
	{
		//generate random slice
		int totalWeight{ m_TotalWeight };

		//Set the slices to know which one is next -> no 2 river pieces after each other
		std::map<TerrainType, int> consideredSlices{};
		for (const auto& pair : m_TerrainWeights)
		{
			switch (pair.first)
			{
			case TerrainType::GRASS:
				consideredSlices.insert(pair);
				break;
			case TerrainType::ROAD:
				consideredSlices.insert(pair);
				break;
			case TerrainType::WATER:
				if (m_PreviousTerrainType == TerrainType::WATER)
				{
					totalWeight -= pair.second;
				}
				else
				{
					consideredSlices.insert(pair);
				}
				break;
			case TerrainType::RAILROAD:
				consideredSlices.insert(pair);
			default:
				break;

			}
		}
		for (const auto& pair : consideredSlices)
		{
			if (rand() % totalWeight < pair.second)
			{
				switch (pair.first)
				{
				case TerrainType::GRASS:
					slice = new GrassSlice(2, m_MaxWidth, m_GrassTextureID);
					AddChild(slice);
					m_PreviousTerrainType = TerrainType::GRASS;
					break;
				case TerrainType::ROAD:
					slice = new RoadSlice(0, m_MaxWidth, m_RoadTextureID);
					AddChild(slice);
					m_PreviousTerrainType = TerrainType::ROAD;
					break;
				case TerrainType::WATER:
					slice = new WaterSlice(4, m_MaxWidth, m_WaterTextureID);
					AddChild(slice);
					m_PreviousTerrainType = TerrainType::WATER;
					break;
				case TerrainType::RAILROAD:
					slice = new RailRoadSlice(0, m_MaxWidth, m_RailroadTextureID);
					AddChild(slice);
					m_PreviousTerrainType = TerrainType::RAILROAD;
					break;
				default:
					break;
				}
				break;
			}
			else
			{
				totalWeight -= pair.second;
			}
		}
	}


	if (slice != nullptr) {
		m_TerrainSlices.insert(std::pair(m_CurrentSliceNumber, (slice)));
		slice->GetTransform()->Translate(0.f, -0.4f, float(m_CurrentSliceNumber));
		//slice->GetTransform()->Scale(10.f);
		++m_CurrentSliceNumber;

	}

}
