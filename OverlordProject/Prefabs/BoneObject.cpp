#include "stdafx.h"
#include "BoneObject.h"

BoneObject::BoneObject(BaseMaterial* pMaterial, float length)
	:m_pMaterial(pMaterial), m_Length(length)
{
}

void BoneObject::AddBone(BoneObject* pBone)
{
	pBone->GetTransform()->Translate(m_Length, 0, 0);
	AddChild(pBone);
}

void BoneObject::CalculateBindPose()
{
	XMStoreFloat4x4(&m_BindPose, XMMatrixInverse(nullptr, XMLoadFloat4x4(&GetTransform()->GetWorld())));

	for (BoneObject* pChildBone : GetChildren<BoneObject>())
	{
		pChildBone->CalculateBindPose();
	}
}

void BoneObject::Initialize(const SceneContext& sceneContext)
{
	GameObject::Initialize(sceneContext);
	GameObject* pEmpty = new GameObject();

	auto* model = pEmpty->AddComponent(new ModelComponent(L"../Resources/Meshes/Bone.ovm"));

	model->SetMaterial(m_pMaterial);
	pEmpty->GetTransform()->Rotate(XMFLOAT3{ 0,-90,0 });
	pEmpty->GetTransform()->Scale(m_Length);

	AddChild(pEmpty);
}
