#include "stdafx.h"
#include "Car.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow.h"

UINT Car::m_OrangeTextureID = 0;
UINT Car::m_YellowTextureID = 0;
UINT Car::m_PurpleTextureID = 0;
UINT Car::m_GreenTextureID = 0;
UINT Car::m_BlueTextureID = 0;

Car::Car(float speed, int direction)
	:m_Speed(speed)
	, m_Direction(direction)
{
}

Car::~Car()
{
}

void Car::Initialize(const SceneContext&)
{
	if (m_PurpleTextureID == 0 || m_YellowTextureID == 0 || m_OrangeTextureID == 0 || m_GreenTextureID == 0 || m_BlueTextureID == 0)
	{
		InitializeTextures();
	}

	ChooseRandomCar();
	//set the collider
}

void Car::Update(const SceneContext& sceneContext)
{
	//move the car model
	float XMovement = m_Speed * sceneContext.pGameTime->GetElapsed() * m_Direction;
	GetTransform()->Translate(GetTransform()->GetPosition().x + XMovement, GetTransform()->GetPosition().y, 0.f);
	//move the collider
}

void Car::InitializeTextures()
{
	//setting textures if they are not set yet
	if (m_OrangeTextureID == 0)
	{
		auto material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
		//Initializing textures for the terrain
		material->SetDiffuseTexture(L"../Resources/Textures/crossy/Vehicle/orange_car.png");
		m_OrangeTextureID = material->GetMaterialId();
	}

	if (m_PurpleTextureID == 0)
	{
		auto material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
		material->SetDiffuseTexture(L"../Resources/Textures/crossy/Vehicle/purple_car.png");
		m_PurpleTextureID = material->GetMaterialId();
	}

	if (m_YellowTextureID == 0)
	{
		auto material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
		material->SetDiffuseTexture(L"../Resources/Textures/crossy/Vehicle/yellow_car.png");
		m_YellowTextureID = material->GetMaterialId();
	}

	if (m_GreenTextureID == 0)
	{
		auto material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
		material->SetDiffuseTexture(L"../Resources/Textures/crossy/Vehicle/green_car.png");
		m_GreenTextureID = material->GetMaterialId();
	}

	if (m_BlueTextureID == 0)
	{
		auto material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
		material->SetDiffuseTexture(L"../Resources/Textures/crossy/Vehicle/blue_car.png");
		m_BlueTextureID = material->GetMaterialId();
	}
}

void Car::ChooseRandomCar()
{
	carModel random = carModel(rand() % 5);
	BaseMaterial* material = nullptr;
	ModelComponent* pModel = nullptr;

	switch (random)
	{
	case carModel::Orange:
		pModel = AddComponent(new ModelComponent(L"../Resources/Meshes/CrossyRoad/Vehicle/orange_car.ovm"));
		material = MaterialManager::Get()->GetMaterial(m_OrangeTextureID);
		pModel->SetMaterial(material);
		break;
	case carModel::Blue:
		pModel = AddComponent(new ModelComponent(L"../Resources/Meshes/CrossyRoad/Vehicle/blue_car.ovm"));
		material = MaterialManager::Get()->GetMaterial(m_BlueTextureID);
		pModel->SetMaterial(material);
		break;
	case carModel::Green:
		pModel = AddComponent(new ModelComponent(L"../Resources/Meshes/CrossyRoad/Vehicle/green_car.ovm"));
		material = MaterialManager::Get()->GetMaterial(m_GreenTextureID);
		pModel->SetMaterial(material);
		break;
	case carModel::Yellow:
		pModel = AddComponent(new ModelComponent(L"../Resources/Meshes/CrossyRoad/Vehicle/yellow_car.ovm"));
		material = MaterialManager::Get()->GetMaterial(m_YellowTextureID);
		pModel->SetMaterial(material);
		break;
	case carModel::Purple:
		pModel = AddComponent(new ModelComponent(L"../Resources/Meshes/CrossyRoad/Vehicle/purple_car.ovm"));
		material = MaterialManager::Get()->GetMaterial(m_PurpleTextureID);
		pModel->SetMaterial(material);
		break;
	default:;
	}

	GetTransform()->Scale(1.f);
	GetTransform()->Rotate(0.f, -m_Direction * 90.f, 0.f, true);
}
