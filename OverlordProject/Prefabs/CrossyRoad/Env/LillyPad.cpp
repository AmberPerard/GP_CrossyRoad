#include "stdafx.h"
#include "LillyPad.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow.h"

void LillyPad::Initialize(const SceneContext& /*sceneContext*/)
{
	ModelComponent* pModel = AddComponent(new ModelComponent(L"../Resources/Meshes/CrossyRoad/env/lillypad.ovm"));
	if(!m_LilyTextureID)
	{
		DiffuseMaterial_Shadow* pMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
		pMat->SetDiffuseTexture(L"../Resources/Textures/crossy/env/Lillypad.png");
		m_LilyTextureID = pMat->GetMaterialId();
	}
	pModel->SetMaterial(MaterialManager::Get()->GetMaterial(m_LilyTextureID));
}
