#include "stdafx.h"
#include "Tree.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"

 UINT Tree::m_SmallTextureID = 0;
 UINT Tree::m_TallTextureID = 0;

void Tree::Initialize(const SceneContext&)
{
	//make textures only once if they don't exist yet
	if (m_SmallTextureID == 0)
	{
		auto materialSmallTree = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
		materialSmallTree->SetDiffuseTexture(L"../Resources/Textures/crossy/env/tree_small.png");
		m_SmallTextureID = materialSmallTree->GetMaterialId();
	}

	if (m_TallTextureID == 0)
	{
		auto materialTallTree = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
		materialTallTree->SetDiffuseTexture(L"../Resources/Textures/crossy/env/tree_tall.png");
		m_TallTextureID = materialTallTree->GetMaterialId();
	}
	//choose a random size
	TreeSize randomSize = static_cast<TreeSize>(rand() % 2);

	ModelComponent* pModel = {};
	m_pObject = new GameObject();

	switch (randomSize)
	{
	case Small:
		pModel = new ModelComponent(L"../Resources/Meshes/CrossyRoad/env/tree_small.ovm");
		pModel->SetMaterial(MaterialManager::Get()->GetMaterial(m_SmallTextureID));
		break;
	case Tall:
		pModel = new ModelComponent(L"../Resources/Meshes/CrossyRoad/env/tree_tall.ovm");
		pModel->SetMaterial(MaterialManager::Get()->GetMaterial(m_TallTextureID));
		break;
	default:
		pModel = new ModelComponent(L"../Resources/Meshes/CrossyRoad/env/tree_small.ovm");
		pModel->SetMaterial(MaterialManager::Get()->GetMaterial(m_SmallTextureID));
		break;
	}

	AddChild(m_pObject);
	m_pObject->AddComponent(pModel);
	m_pObject->GetTransform()->Scale(1.f);
}
