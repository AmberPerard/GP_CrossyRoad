#include "stdafx.h"
#include "CrossyCharacter.h"
#include "Materials/DiffuseMaterial.h"

CrossyCharacter::CrossyCharacter(CharacterDesc characterDesc)
	:m_CharacterDesc{ characterDesc }
{
	const auto material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	material->SetDiffuseTexture(L"../Resources/Textures/Chicken.png");

	m_pCharachter = new GameObject();
	auto model = m_pCharachter->AddComponent(new ModelComponent(L"../Resources/Meshes/CrossyRoad/Chicken.ovm"));
	model->SetMaterial(material);

	m_pCharachter->GetTransform()->Scale(15.f);
	AddChild(m_pCharachter);
	m_JumpTimer = m_JumpTime;
}

void CrossyCharacter::Initialize(const SceneContext& sceneContext)
{
	GameObject::Initialize(sceneContext);
}

void CrossyCharacter::Update(const SceneContext& sceneContext)
{
	GameObject::Update(sceneContext);
	
	bool isMoving = false;

	if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveForward)
		|| sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveBackward)
		|| sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveLeft)
		|| sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveRight))
		isMoving = true;

	if (m_JumpTimer <= 0.f)
	{
		m_PrevPos = m_TargetPos;

		if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveForward))
		{
			m_TargetPos.x += m_TileSize;
			SetRotation(180.f);
		}

		if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveBackward))
		{
			m_TargetPos.x -= m_TileSize;
			SetRotation(0.f);
		}

		if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveRight))
		{
			m_TargetPos.y -= m_TileSize;
			SetRotation(270.f);
		}

		if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveLeft))
		{
			m_TargetPos.y += m_TileSize;
			SetRotation(90.f);
		}

		if (isMoving)
		{
			m_JumpTimer = m_JumpTime;
		}
	}
	else
	{
		if (m_JumpTimer >= 0.f)
		{
			m_JumpTimer -= sceneContext.pGameTime->GetElapsed();
			MathHelper::Clamp(m_JumpTimer, m_JumpTime, 0.f);
		}
	}

	//implement Squish

	MoveCharacter();
	RotateCharacter(sceneContext);

}

void CrossyCharacter::DrawImGui()
{
	//ImGui::Begin("Character");
	//ImGui::DragFloat("JumpTime: %f", m_JumpTime);
	//ImGui::Text("TileSize: %f", m_TileSize);
	//ImGui::Text("RotationTime: %f", m_RotationTime);
	//ImGui::Text("MaxSquishScale: %f", m_MaxSquishScale);
	//ImGui::End();
}

void CrossyCharacter::MoveCharacter()
{
	//Lerp over the jump so it looks smooth
	float jumpProgress = 1.f - (m_JumpTimer / m_JumpTime);
	if (jumpProgress < 0.5f)
	{
		//move the character up
		m_CurrentHeight = std::lerp(0.f, m_JumpHeight, jumpProgress * 2);
	}
	else
	{
		//move the character down
		float targetHeight = 0.f;
		m_CurrentHeight = std::lerp(m_JumpHeight, targetHeight, (jumpProgress - 0.5f) * 2);
	}

	MathHelper::Clamp(jumpProgress, 1.f, 0.f);
	float xPos = std::lerp(m_PrevPos.x, m_TargetPos.x, jumpProgress);
	float zPos = std::lerp(m_PrevPos.y, m_TargetPos.y, jumpProgress);

	GetTransform()->Translate(xPos, m_CurrentHeight, zPos);
}

void CrossyCharacter::RotateCharacter(const SceneContext& sceneContext)
{
	//Lerp over the rotation so it looks smooth
	m_RotationTimer -= sceneContext.pGameTime->GetElapsed();
	float rotationLerp{ 1 - (m_RotationTimer / m_RotationTime) };
	MathHelper::Clamp(rotationLerp, 1.f, 0.f);
	float currentRotation = std::lerp(m_PrevRotationY, m_TargetRotationY, rotationLerp);
	GetTransform()->Rotate(0.f, currentRotation, 0.f);
}

void CrossyCharacter::SetRotation(float rotation)
{
	m_PrevRotationY = m_TargetRotationY;
	m_TargetRotationY = rotation;

	if (fmod(m_PrevRotationY, 360.f) < 0.f)
	{
		m_PrevRotationY = fmod(m_PrevRotationY, 360.f) + 360.f;
	}

	float delta = (m_TargetRotationY - m_PrevRotationY);

	if (abs(delta) > 180.f)
	{
		if (delta < 0.f)
		{
			m_PrevRotationY -= 360.f;
		}
		else
		{
			m_PrevRotationY += 360.f;
		}
	}
	m_RotationTimer = m_RotationTime;
}
