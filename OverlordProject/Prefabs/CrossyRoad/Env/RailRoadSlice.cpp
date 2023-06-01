#include "stdafx.h"
#include "RailRoadSlice.h"

RailRoadSlice::RailRoadSlice(int amountOfObstacles, int maxWidth, UINT textureID)
	:TerrainSlice(amountOfObstacles, maxWidth,textureID)
{
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
	auto mat = MaterialManager::Get()->GetMaterial(m_TextureID);

	model->SetMaterial(mat);
	AddChild(m_pSlice);

	//have trains moving on the rails
}
