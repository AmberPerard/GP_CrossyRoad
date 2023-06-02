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

	m_pCarCollision->SetOnTriggerCallBack([=](GameObject* pTrigger, GameObject* pOther, PxTriggerAction action)
		{
			OnCollision(pTrigger, pOther, action);
		});

}

void Car::Update(const SceneContext& sceneContext)
{
	//move the car model
	float XMovement = m_Speed * sceneContext.pGameTime->GetElapsed() * m_Direction;
	GetTransform()->Translate(GetTransform()->GetPosition().x + XMovement, GetTransform()->GetPosition().y, GetTransform()->GetPosition().z);

	//align collider with the car
	XMFLOAT3 pos = GetTransform()->GetWorldPosition();
	m_pCarCollision->GetTransform()->Translate(pos.x, pos.y, pos.z);
}

void Car::OnCollision(GameObject*, GameObject* pOther, PxTriggerAction action)
{
	if (action == PxTriggerAction::ENTER)
	{
		auto pCharacter = dynamic_cast<CrossyCharacter*>(pOther);
		if (pCharacter != nullptr)
		{
			pCharacter->SetDead(true);
		}
	}
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
	BaseMaterial* material = nullptr;
	ModelComponent* pModel = nullptr;

	//set basics for the collision of the car
	m_pCarCollision = new GameObject();
	m_pCarCollision->GetTransform()->Translate(GetTransform()->GetPosition().x, GetTransform()->GetPosition().y, GetTransform()->GetPosition().z);
	RigidBodyComponent* pRigid = nullptr;
	physx::PxConvexMesh* pConvexCarMesh = nullptr;
	PxMaterial* pDefaultMat = PxGetPhysics().createMaterial(0.f, 0.f, 0.f);

	switch (random)
	{
	case carModel::Orange:
		pModel = AddComponent(new ModelComponent(L"../Resources/Meshes/CrossyRoad/Vehicle/orange_car.ovm"));
		material = MaterialManager::Get()->GetMaterial(m_OrangeTextureID);
		pModel->SetMaterial(material);
		pRigid = m_pCarCollision->AddComponent(new RigidBodyComponent());
		pConvexCarMesh = ContentManager::Load<PxConvexMesh>(L"../Resources/Meshes/CrossyRoad/Vehicle/orange_car.ovpc");
		pRigid->AddCollider(PxConvexMeshGeometry(pConvexCarMesh, PxMeshScale(10)), *pDefaultMat, true);

		break;
	case carModel::Blue:
		pModel = AddComponent(new ModelComponent(L"../Resources/Meshes/CrossyRoad/Vehicle/blue_car.ovm"));
		material = MaterialManager::Get()->GetMaterial(m_BlueTextureID);
		pModel->SetMaterial(material);
		pRigid = m_pCarCollision->AddComponent(new RigidBodyComponent());
		pConvexCarMesh = ContentManager::Load<PxConvexMesh>(L"../Resources/Meshes/CrossyRoad/Vehicle/blue_car.ovpc");
		pRigid->AddCollider(PxConvexMeshGeometry(pConvexCarMesh, PxMeshScale(10)), *pDefaultMat, true);
		break;
	case carModel::Green:
		pModel = AddComponent(new ModelComponent(L"../Resources/Meshes/CrossyRoad/Vehicle/green_car.ovm"));
		material = MaterialManager::Get()->GetMaterial(m_GreenTextureID);
		pModel->SetMaterial(material);
		pRigid = m_pCarCollision->AddComponent(new RigidBodyComponent());
		pConvexCarMesh = ContentManager::Load<PxConvexMesh>(L"../Resources/Meshes/CrossyRoad/Vehicle/green_car.ovpc");
		pRigid->AddCollider(PxConvexMeshGeometry(pConvexCarMesh, PxMeshScale(10)), *pDefaultMat, true);
		break;
	case carModel::Yellow:
		pModel = AddComponent(new ModelComponent(L"../Resources/Meshes/CrossyRoad/Vehicle/yellow_car.ovm"));
		material = MaterialManager::Get()->GetMaterial(m_YellowTextureID);
		pModel->SetMaterial(material);
		pRigid = m_pCarCollision->AddComponent(new RigidBodyComponent());
		pConvexCarMesh = ContentManager::Load<PxConvexMesh>(L"../Resources/Meshes/CrossyRoad/Vehicle/yellow_car.ovpc");
		pRigid->AddCollider(PxConvexMeshGeometry(pConvexCarMesh, PxMeshScale(10)), *pDefaultMat, true);
		break;
	case carModel::Purple:
		pModel = AddComponent(new ModelComponent(L"../Resources/Meshes/CrossyRoad/Vehicle/purple_car.ovm"));
		material = MaterialManager::Get()->GetMaterial(m_PurpleTextureID);
		pModel->SetMaterial(material);
		pRigid = m_pCarCollision->AddComponent(new RigidBodyComponent());
		pConvexCarMesh = ContentManager::Load<PxConvexMesh>(L"../Resources/Meshes/CrossyRoad/Vehicle/purple_car.ovpc");
		pRigid->AddCollider(PxConvexMeshGeometry(pConvexCarMesh, PxMeshScale(10)), *pDefaultMat, true);
		break;
	default:;
	}
	GetTransform()->Scale(1.f);
	GetTransform()->Rotate(0.f, -m_Direction * 90.f, 0.f, true);
	m_pCarCollision->GetTransform()->Rotate(0.f, -m_Direction * 90.f, 0.f, true);
	AddChild(m_pCarCollision);
}
