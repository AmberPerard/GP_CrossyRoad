#include "stdafx.h"
#include "SoftwareSkinningScene_1.h"

#include "Materials/ColorMaterial.h"
#include "Prefabs/BoneObject.h"

void SoftwareSkinningScene_1::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;

	ColorMaterial* pColorMaterial = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	pColorMaterial->SetVariable_Vector(L"gColor", XMFLOAT4{ Colors::Gold });

	GameObject* pRoot = new GameObject();

	m_pBone0 = new BoneObject(pColorMaterial, 15.f);
	pRoot->AddChild(m_pBone0);

	m_pBone1 = new BoneObject(pColorMaterial, 15.f);
	m_pBone0->AddBone(m_pBone1);

	AddChild(pRoot);
}

void SoftwareSkinningScene_1::Update()
{
	GameScene::Update();
	if (m_AutoRotate)
	{
		static float dTime = 0;
		dTime += GetSceneContext().pGameTime->GetElapsed();
		m_BoneRotation = sin(dTime) * 45;
	}

	m_pBone0->GetTransform()->Rotate(0, 0, m_BoneRotation);
	m_pBone1->GetTransform()->Rotate(0, 0, -m_BoneRotation * 2.f);
}

void SoftwareSkinningScene_1::OnGUI()
{
	GameScene::OnGUI();
	ImGui::DragFloat("bone rotation", &m_BoneRotation, 0.2f, -45.f, 45.f);
	ImGui::Checkbox("Auto Rotate", &m_AutoRotate);
}
