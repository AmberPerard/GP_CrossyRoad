#include "stdafx.h"
#include "WaterSlice.h"
#include "GrassSlice.h"
#include "LillyPad.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Prefabs/CrossyRoad/TerrainGenerator.h"

WaterSlice::WaterSlice(int amountOfLilyPads, int maxWidth, UINT textureID)
	: TerrainSlice(amountOfLilyPads, maxWidth, textureID)
{
}

WaterSlice::~WaterSlice()
{

}

bool WaterSlice::IsSlicePassable(int /*xPos*/)
{
	return true;
}

bool WaterSlice::IsLilyPad(int xPos)
{
	if (m_pLilyPads.find(xPos) != m_pLilyPads.end())
	{
		return true;
	}
	return false;
}

void WaterSlice::Initialize(const SceneContext&)
{
	m_pSlice = new GameObject();
	auto model = m_pSlice->AddComponent(new ModelComponent(L"../Resources/Meshes/CrossyRoad/env/slice.ovm"));
	DiffuseMaterial_Shadow* mat = dynamic_cast<DiffuseMaterial_Shadow*>(MaterialManager::Get()->GetMaterial(m_TextureID));

	model->SetMaterial(mat);
	AddChild(m_pSlice);
	//Create the Lilypads
	//if the previous slice is a grass slice, spawn lily pads not at the same x pos as the obstacles

	std::vector<int> usedNumbers{};
	TerrainGenerator* pTerrain = dynamic_cast<TerrainGenerator*>(GetParent());
	GrassSlice* pGrass = dynamic_cast<GrassSlice*>(pTerrain->GetPreviousTerrain());
	if (pGrass)
	{
		for (int i{ -m_MaxWidth }; i <= m_MaxWidth; ++i)
		{
			if (!pGrass->IsSlicePassable(i))
			{
				usedNumbers.emplace_back(i);
			}
		}
	}

	GameObject* pLily{ nullptr };
	//spawn obstacles at random locations
	for (int i{}; i < m_NrObstacles; ++i)
	{
		int randomNumber{};
		while (true)
		{
			randomNumber = (rand() % (m_MaxWidth * 2 + 1)) - m_MaxWidth;
			if (std::find(usedNumbers.begin(), usedNumbers.end(), randomNumber) == usedNumbers.end())
			{
				pLily = AddChild(new LillyPad());
				m_pLilyPads.insert(std::pair{ randomNumber, pLily });
				pLily->GetTransform()->Translate(XMFLOAT3{ static_cast<float>(randomNumber), 0.40f, 0.f });


				//ad number to used numbers :)
				usedNumbers.emplace_back(randomNumber);
				break;
			}

		}
	}
}
