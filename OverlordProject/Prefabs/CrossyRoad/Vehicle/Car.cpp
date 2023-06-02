#include "stdafx.h"
#include "Car.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow.h"

Car::Car(float speed, int direction)
	:m_Speed(speed)
	,m_Direction(direction)
{
}

Car::~Car()
{
}

void Car::Initialize(const SceneContext&)
{
	//get the car model
	auto model = AddComponent(new ModelComponent(L"../Resources/Meshes/CrossyRoad/Vehicle/purple_car.ovm"));
	GetTransform()->Scale(1.f);
	GetTransform()->Rotate(0.f, 90.f * -m_Direction, 0.f, true);

	//set the material
	auto material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	material->SetDiffuseTexture(L"../Resources/Textures/crossy/Vehicle/purple_car.png");
	model->SetMaterial(material);



	//set the collider
}

void Car::Update(const SceneContext&)
{
	//move the car model

	//move the collider
}
