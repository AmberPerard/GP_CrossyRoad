#include "stdafx.h"
#include "RoadSlice.h"

RoadSlice::RoadSlice(int amountOfObstacles, int maxWidth, UINT textureID)
	:TerrainSlice(amountOfObstacles, maxWidth, textureID)
{
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
	auto mat = MaterialManager::Get()->GetMaterial(m_TextureID);
	model->SetMaterial(mat);
	AddChild(m_pSlice);

	//Create the cars that will move on the road
}
