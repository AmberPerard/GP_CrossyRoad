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
	m_pCarMesh->GetTransform()->Translate(GetTransform()->GetPosition().x + XMovement, GetTransform()->GetPosition().y, 0.f);

	//move the collider
	m_pCarCollision->GetTransform()->Translate(GetTransform()->GetPosition().x + XMovement, GetTransform()->GetPosition().y, 0.f);
}

void Car::InitializeTextures()
{
	//setting textures if they are not set yet
	if (m_OrangeTextureID == 0)
	{
		auto material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
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
	m_pCarCollision = new GameObject();
	m_pCarMesh = new GameObject();
	BaseMaterial* material = nullptr;
	ModelComponent* pModel = nullptr;
	RigidBodyComponent* pRigid = nullptr;
	physx::PxConvexMesh* pConvexCarMesh = nullptr;
	PxMaterial* pDefaultMat = PxGetPhysics().createMaterial(0.f, 0.f, 0.f);

	switch (random)
	{
	case carModel::Orange:
		pModel = m_pCarCollision->AddComponent(new ModelComponent(L"../Resources/Meshes/CrossyRoad/Vehicle/orange_car.ovm"));
		material = MaterialManager::Get()->GetMaterial(m_OrangeTextureID);
		pRigid = m_pCarMesh->AddComponent(new RigidBodyComponent());
		pConvexCarMesh = ContentManager::Load<PxConvexMesh>(L"../Resources/Meshes/CrossyRoad/Vehicle/orange_car.ovpc");
		pRigid->AddCollider(PxConvexMeshGeometry(pConvexCarMesh, PxMeshScale(10)), *pDefaultMat); pModel->SetMaterial(material);
		break;
	case carModel::Blue:
		pModel = m_pCarCollision->AddComponent(new ModelComponent(L"../Resources/Meshes/CrossyRoad/Vehicle/blue_car.ovm"));
		material = MaterialManager::Get()->GetMaterial(m_BlueTextureID);
		pModel->SetMaterial(material);
		pRigid = m_pCarMesh->AddComponent(new RigidBodyComponent());
		pConvexCarMesh = ContentManager::Load<PxConvexMesh>(L"../Resources/Meshes/CrossyRoad/Vehicle/blue_car.ovpc");
		pRigid->AddCollider(PxConvexMeshGeometry(pConvexCarMesh, PxMeshScale(10)), *pDefaultMat);
		break;
	case carModel::Green:
		pModel = m_pCarCollision->AddComponent(new ModelComponent(L"../Resources/Meshes/CrossyRoad/Vehicle/green_car.ovm"));
		material = MaterialManager::Get()->GetMaterial(m_GreenTextureID);
		pModel->SetMaterial(material);
		pRigid = m_pCarMesh->AddComponent(new RigidBodyComponent());
		pConvexCarMesh = ContentManager::Load<PxConvexMesh>(L"../Resources/Meshes/CrossyRoad/Vehicle/green_car.ovpc");
		pRigid->AddCollider(PxConvexMeshGeometry(pConvexCarMesh, PxMeshScale(10)), *pDefaultMat);
		break;
	case carModel::Yellow:
		pModel = m_pCarCollision->AddComponent(new ModelComponent(L"../Resources/Meshes/CrossyRoad/Vehicle/yellow_car.ovm"));
		material = MaterialManager::Get()->GetMaterial(m_YellowTextureID);
		pModel->SetMaterial(material);
		pRigid = m_pCarMesh->AddComponent(new RigidBodyComponent());
		pConvexCarMesh = ContentManager::Load<PxConvexMesh>(L"../Resources/Meshes/CrossyRoad/Vehicle/yellow_car.ovpc");
		pRigid->AddCollider(PxConvexMeshGeometry(pConvexCarMesh, PxMeshScale(10)), *pDefaultMat);
		break;
	case carModel::Purple:
		pModel = m_pCarCollision->AddComponent(new ModelComponent(L"../Resources/Meshes/CrossyRoad/Vehicle/purple_car.ovm"));
		material = MaterialManager::Get()->GetMaterial(m_PurpleTextureID);
		pModel->SetMaterial(material);
		pRigid = m_pCarMesh->AddComponent(new RigidBodyComponent());
		pConvexCarMesh = ContentManager::Load<PxConvexMesh>(L"../Resources/Meshes/CrossyRoad/Vehicle/purple_car.ovpc");
		pRigid->AddCollider(PxConvexMeshGeometry(pConvexCarMesh, PxMeshScale(10)), *pDefaultMat);
		break;
	default:;
	}
	m_pCarMesh->GetTransform()->Scale(1.f);
	m_pCarMesh->GetTransform()->Rotate(0.f, -m_Direction * 90.f, 0.f, true);
	AddChild(m_pCarCollision);
	AddChild(m_pCarMesh);
}
