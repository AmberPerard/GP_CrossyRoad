#include "stdafx.h"
#include "TerrainGenerator.h"
#include "TerrainSlice.h"
#include "GrassSlice.h"
#include "Materials/DiffuseMaterial.h"

TerrainGenerator::TerrainGenerator(CrossyCharacter* trackedCharacter, int slicesAhead, int width)
	: m_TrackedCharacter(trackedCharacter)
	, m_SlicesAhead(slicesAhead)
	, m_MaxWidth(width)
{
}

TerrainGenerator::~TerrainGenerator()
{
}

void TerrainGenerator::Reset()
{
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

bool TerrainGenerator::IsCurrentSlicePassable(int /*xPos*/, int sliceNr)
{
	TerrainSlice* pSlice = m_TerrainSlices.find(sliceNr)->second;
	if (pSlice)
	{
		//return the pslice.passable()
	}

	return true;
}

void TerrainGenerator::Initialize(const SceneContext& /*sceneContext*/)
{
	//Initializing textures for the terrain
	auto material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	material->SetDiffuseTexture(L"../Resources/Textures/crossy/env/light-grass.png");
	m_GrassTextureID = material->GetMaterialId();

	material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	material->SetDiffuseTexture(L"../Resources/Textures/crossy/env/water.png");
	m_WaterTextureID = material->GetMaterialId();

	material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	material->SetDiffuseTexture(L"../Resources/Textures/crossy/env/road.png");
	m_RoadTextureID = material->GetMaterialId();

	material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	material->SetDiffuseTexture(L"../Resources/Textures/crossy/env/railroad.png");
	m_RailroadTextureID = material->GetMaterialId();


	//put a few grass slices down by default for the player to start on
	//first 5 slices before slice 0
	for (int i{ -5 }; i < 0; ++i)
	{
		TerrainSlice* slice = nullptr;
		auto grass = new GrassSlice(100, m_MaxWidth, m_GrassTextureID);
		slice = AddChild(grass);
		m_TerrainSlices.insert({ i, slice });
		slice->GetTransform()->Translate(0.f, 0.f, (float)i);
	}
}

void TerrainGenerator::Update(const SceneContext& sceneContext)
{
	GameObject::Update(sceneContext);
}

void TerrainGenerator::SpawnNewSlice()
{
}
