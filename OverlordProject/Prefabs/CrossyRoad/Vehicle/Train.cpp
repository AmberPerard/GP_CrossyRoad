#include "stdafx.h"
#include "Train.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Prefabs/CrossyRoad/CrossyCharacter.h"

UINT Train::m_TrainFrontTextureID = 0;
UINT Train::m_TrainMiddleTextureID = 0;
UINT Train::m_TrainEndTextureID = 0;

Train::Train(float speed, int direction, float xPos, TrainPiece whatTrainPiece)
	: m_Speed(speed)
	, m_Direction(direction)
	, m_xPos(xPos)
	, m_WhatTrainPiece(whatTrainPiece)
	, m_pTrainCollision(nullptr)
{
}

Train::~Train()
{
}

void Train::Initialize(const SceneContext&)
{
	if (m_TrainFrontTextureID == 0 || m_TrainMiddleTextureID == 0 || m_TrainEndTextureID == 0)
	{
		InitializeTextures();
	}

	ChooseCorrectTrainPiece();

	GetTransform()->Scale(1.f);
	GetTransform()->Rotate(0.f, float(m_Direction) * 180.f, 0.f, true);
	m_pTrainCollision->GetTransform()->Rotate(0.f, float(m_Direction) * 180.f, 0.f, true);
	GetTransform()->Translate(m_xPos, 0.2f, 0.f);

	m_pTrainCollision->SetOnTriggerCallBack([=](GameObject* pTrigger, GameObject* pOther, PxTriggerAction action)
		{
			OnCollision(pTrigger, pOther, action);
		});

	FMOD::System* pFmod = SoundManager::Get()->GetSystem();
	pFmod->createStream("Resources/Audio/trainsplat.wav", FMOD_DEFAULT, nullptr, &m_pTrainDeath);
	pFmod->createStream("Resources/Audio/Train_Alarm.wav", FMOD_DEFAULT, nullptr, &m_pTrainAlarm);
	pFmod->createStream("Resources/Audio/train_pass_no_horn.wav", FMOD_DEFAULT, nullptr, &m_pTrainPassNoHorn);
	pFmod->createStream("Resources/Audio/train_pass_shorter.wav", FMOD_DEFAULT, nullptr, &m_pTrainPassHorn);

	SoundManager::Get()->GetSystem()->playSound(m_pTrainAlarm, nullptr, false, &m_pChannelAlarm);
	m_pChannelAlarm->setVolume(0.2f);

}

void Train::Update(const SceneContext& sceneContext)
{
	if(!m_PassPlayed)
	{
		//SoundManager::Get()->GetSystem()->playSound(m_pTrainPassHorn, nullptr, false, &m_pChannelTrainPass);
	//	m_pChannelTrainPass->setVolume(0.2f);
	}
	float XMovement = m_Speed * sceneContext.pGameTime->GetElapsed() * m_Direction;
	GetTransform()->Translate(GetTransform()->GetPosition().x + XMovement, GetTransform()->GetPosition().y, GetTransform()->GetPosition().z);

	//align collider with the train
	XMFLOAT3 pos = GetTransform()->GetWorldPosition();
	m_pTrainCollision->GetTransform()->Translate(pos.x, pos.y, pos.z);
}

void Train::OnCollision(GameObject*, GameObject* pOther, PxTriggerAction action)
{
	if (action == PxTriggerAction::ENTER)
	{
		auto pCharacter = dynamic_cast<CrossyCharacter*>(pOther);
		if (pCharacter != nullptr)
		{
			pCharacter->SetDead(true);
			SoundManager::Get()->GetSystem()->playSound(m_pTrainDeath, nullptr, false, &m_pChannelDeathTrain);
			m_pChannelDeathTrain->setVolume(0.6f);
		}
	}
}

void Train::InitializeTextures()
{
	//setting textures if they are not set yet
	if (m_TrainFrontTextureID == 0)
	{
		auto material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
		material->SetDiffuseTexture(L"../Resources/Textures/crossy/Vehicle/front_train.png");
		m_TrainFrontTextureID = material->GetMaterialId();
	}

	if (m_TrainMiddleTextureID == 0)
	{
		auto material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
		material->SetDiffuseTexture(L"../Resources/Textures/crossy/Vehicle/middle_train.png");
		m_TrainMiddleTextureID = material->GetMaterialId();
	}

	if (m_TrainEndTextureID == 0)
	{
		auto material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
		material->SetDiffuseTexture(L"../Resources/Textures/crossy/Vehicle/back_train.png");
		m_TrainEndTextureID = material->GetMaterialId();
	}
}

void Train::ChooseCorrectTrainPiece()
{
	BaseMaterial* material = nullptr;
	ModelComponent* pModel = nullptr;
	switch (m_WhatTrainPiece)
	{
	case TrainPiece::front:
		pModel = AddComponent(new ModelComponent(L"../Resources/Meshes/CrossyRoad/Vehicle/front_train.ovm"));
		material = MaterialManager::Get()->GetMaterial(m_TrainFrontTextureID);
		pModel->SetMaterial(material);
		break;
	case TrainPiece::middle:
		pModel = AddComponent(new ModelComponent(L"../Resources/Meshes/CrossyRoad/Vehicle/middle_train.ovm"));
		material = MaterialManager::Get()->GetMaterial(m_TrainMiddleTextureID);
		pModel->SetMaterial(material);
		break;
	case TrainPiece::end:
		pModel = AddComponent(new ModelComponent(L"../Resources/Meshes/CrossyRoad/Vehicle/back_train.ovm"));
		material = MaterialManager::Get()->GetMaterial(m_TrainEndTextureID);
		pModel->SetMaterial(material);
		break;
	default:;
	}

	//set basics for the collision of the train
	m_pTrainCollision = new GameObject();
	m_pTrainCollision->GetTransform()->Translate(GetTransform()->GetPosition().x, GetTransform()->GetPosition().y, GetTransform()->GetPosition().z);
	RigidBodyComponent* pRigid = m_pTrainCollision->AddComponent(new RigidBodyComponent());
	physx::PxConvexMesh* pConvexTrainMesh = ContentManager::Load<PxConvexMesh>(L"../Resources/Meshes/CrossyRoad/Vehicle/front_train.ovpc");
	PxMaterial* pDefaultMat = PxGetPhysics().createMaterial(0.f, 0.f, 0.f);
	pRigid->AddCollider(PxConvexMeshGeometry(pConvexTrainMesh, PxMeshScale(10)), *pDefaultMat, true);

	AddChild(m_pTrainCollision);
}
