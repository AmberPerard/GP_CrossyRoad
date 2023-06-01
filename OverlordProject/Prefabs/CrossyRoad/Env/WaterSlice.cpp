#include "stdafx.h"
#include "WaterSlice.h"

WaterSlice::WaterSlice(int amountOfLilyPads, int maxWidth, UINT textureID)
	: TerrainSlice(amountOfLilyPads, maxWidth, textureID)
{
}

WaterSlice::~WaterSlice()
{
	for (auto& lilyPad : m_pLilyPads)
	{
		SafeDelete(lilyPad.second);
	}
	m_pLilyPads.clear();
}

bool WaterSlice::IsSlicePassable(int xPos)
{
	if (m_pLilyPads.find(xPos) == m_pLilyPads.end()) {
		return false;
	}
	return true;
}

void WaterSlice::Initialize(const SceneContext&)
{
	m_pSlice = new GameObject();
	auto model = m_pSlice->AddComponent(new ModelComponent(L"../Resources/Meshes/CrossyRoad/env/slice.ovm"));
	auto mat = MaterialManager::Get()->GetMaterial(m_TextureID);

	model->SetMaterial(mat);
	AddChild(m_pSlice);
	//Create the Lilypads down
}
