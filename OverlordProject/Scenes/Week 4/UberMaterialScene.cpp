#include "stdafx.h"
#include "UberMaterialScene.h"
#include "Materials/UberMaterial.h"

void UberMaterialScene::Initialize()
{
	m_SceneContext.settings.drawGrid = false;

	m_SceneContext.settings.enableOnGUI = true;

	m_pMaterial = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	m_pMaterial->SetDiffuseTexture(L"../Resources/Textures/Skulls_Diffusemap.tga");
	m_pMaterial->SetSpecularTexture(L"../Resources/Textures/Skulls_Heightmap.tga");
	m_pMaterial->SetNormalTexture(L"../Resources/Textures/Skulls_Normalmap.tga");
	m_pMaterial->SetCubeMap(L"../Resources/Textures/Sunol_Cubemap.dds");
	m_pMaterial->SetOpacityMap(L"../Resources/Textures/Specular_Level.tga");

	m_pSphere = new GameObject();
	auto* model = m_pSphere->AddComponent(new ModelComponent(L"../Resources/Meshes/Sphere.ovm"));
	model->SetMaterial(m_pMaterial);
	AddChild(m_pSphere);

	m_SceneContext.pCamera->GetTransform()->Translate(0, 3, -5);
}

void UberMaterialScene::Update()
{
	GameScene::Update();
}

void UberMaterialScene::OnGUI()
{
	GameScene::OnGUI();
}
