#include "stdafx.h"
#include "GrassSlice.h"

#include "Tree.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"

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
	DiffuseMaterial_Shadow* mat = dynamic_cast<DiffuseMaterial_Shadow*>(MaterialManager::Get()->GetMaterial(m_TextureID));

	model->SetMaterial(mat);
	AddChild(m_pSlice);
	//Create the obstacles
	//Put the obstacles on the side
	CreateSideTrees();

	if (m_NrObstacles == 100)
	{
		//spawn obstacles at all locations
		for (int i{ -m_MaxWidth }; i < m_MaxWidth + 1; ++i)
		{
			//spawn tree at this x pos
			GameObject* pObstacle = nullptr;
			pObstacle = AddChild(new Tree());
			m_Obstacles.insert(std::pair{ i, pObstacle });
			pObstacle->GetTransform()->Translate(XMFLOAT3{ float(i), 0.4f, 0.f });
		}
	}
	else
	{
		CreateObstacles();
	}
}

void GrassSlice::CreateSideTrees()
{
	//Put the obstacles on the side
	GameObject* pObstacle = nullptr;

	for (int i = 1; i <= 3; i++)
	{
		pObstacle = AddChild(new Tree());
		auto xpos = -m_MaxWidth - float(i);
		pObstacle->GetTransform()->Translate(XMFLOAT3{ xpos, 0.4f, 0.f });
		m_Obstacles.insert(std::pair{ int(xpos), pObstacle });
		pObstacle = AddChild(new Tree());
		xpos = m_MaxWidth + float(i);
		pObstacle->GetTransform()->Translate(XMFLOAT3{ xpos, 0.4f, 0.f });
		m_Obstacles.insert(std::pair{int(xpos), pObstacle });
	}
}

void GrassSlice::CreateObstacles()
{
	//spawn obstacles at random locations
	for (int i{ 0 }; i < m_NrObstacles; ++i)
	{
		//spawn tree at this x pos
		GameObject* pObstacle = nullptr;
		pObstacle = AddChild(new Tree());
		int xpos = rand() % (m_MaxWidth * 2) - m_MaxWidth;
		m_Obstacles.insert(std::pair{ xpos, pObstacle });
		pObstacle->GetTransform()->Translate(XMFLOAT3{ float(xpos), 0.4f, 0.f });
	}
}
