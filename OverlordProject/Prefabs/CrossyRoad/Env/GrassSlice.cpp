#include "stdafx.h"
#include "GrassSlice.h"

GrassSlice::GrassSlice(int amountOfObstacles, int maxWidth, UINT textureID)
	:TerrainSlice(amountOfObstacles, maxWidth, textureID)
{
}

GrassSlice::~GrassSlice()
{

}

bool GrassSlice::IsSlicePassable(int xPos)
{
	if (m_Obstacles.find(xPos) == m_Obstacles.end()) {
		return true;
	}
	return false;
}

void GrassSlice::Initialize(const SceneContext& /*sceneContext*/)
{
	m_pSlice = new GameObject();
	auto model = m_pSlice->AddComponent(new ModelComponent(L"../Resources/Meshes/CrossyRoad/env/slice.ovm"));
	auto mat = MaterialManager::Get()->GetMaterial(m_TextureID);

	model->SetMaterial(mat);
	AddChild(m_pSlice);
	//Create the obstacles
	//Put the obstacles on the side
}
