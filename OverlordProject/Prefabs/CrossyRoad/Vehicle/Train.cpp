#include "stdafx.h"
#include "Train.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow.h"

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
	GetTransform()->Translate(m_xPos, 0.2f, 0.f);
}

void Train::Update(const SceneContext& sceneContext)
{
	float XMovement = m_Speed * sceneContext.pGameTime->GetElapsed() * m_Direction;
	GetTransform()->Translate(GetTransform()->GetPosition().x + XMovement, GetTransform()->GetPosition().y, GetTransform()->GetPosition().z);
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
}
