#include "stdafx.h"
#include "ModelTestScene.h"
#include "Materials/ColorMaterial.h"
#include "Materials/DiffuseMaterial.h"

void ModelTestScene::Initialize()
{
	ColorMaterial* pColorMaterial = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	pColorMaterial->SetVariable_Vector(L"gColor", XMFLOAT4{ Colors::DarkRed });

	DiffuseMaterial* pDiffuseMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pDiffuseMaterial->SetDiffuseTexture(L"../Resources/Textures/Chair_Dark.dds");

	auto pDefaultMat = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.8f);
	GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMat);

	m_pChair = new GameObject();

	auto modelComp = m_pChair->AddComponent(new ModelComponent(L"../Resources/Meshes/Chair.ovm"));
	modelComp->SetMaterial(pDiffuseMaterial);

	auto pRigid = m_pChair->AddComponent(new RigidBodyComponent());
	auto pConvexChair = ContentManager::Load<PxConvexMesh>(L"../Resources/Meshes/Chair.ovpc");
	pRigid->AddCollider(PxConvexMeshGeometry(pConvexChair), *pDefaultMat);

	AddChild(m_pChair);
	m_pChair->GetTransform()->Translate(0.f, 2.f, 0.f);
}

void ModelTestScene::Update()
{
	GameScene::Update();
}

void ModelTestScene::OnGUI()
{
	GameScene::OnGUI();
}
