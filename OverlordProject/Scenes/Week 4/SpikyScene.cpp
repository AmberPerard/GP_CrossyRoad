#include "stdafx.h"
#include "SpikyScene.h"
#include "Materials/SpikeMaterial.h"

void SpikyScene::Initialize()
{
	m_SceneContext.settings.drawGrid = false;

	m_SceneContext.settings.enableOnGUI = true;

	m_pMaterial = MaterialManager::Get()->CreateMaterial<SpikeMaterial>();

	m_pObjects = new GameObject();
	auto* model = m_pObjects->AddComponent(new ModelComponent(L"../Resources/Meshes/OctaSphere.ovm"));
	model->SetMaterial(m_pMaterial);
	AddChild(m_pObjects);

	m_pObjects->GetTransform()->Scale(15.f);
}

void SpikyScene::Update()
{
	GameScene::Update();
}

void SpikyScene::OnGUI()
{
	GameScene::OnGUI();
	m_pMaterial->DrawImGui();
}
