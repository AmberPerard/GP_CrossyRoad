#include "stdafx.h"
#include "CrossyCharacter.h"

#include "TerrainGenerator.h"
#include "Env/WaterSlice.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"

CrossyCharacter::CrossyCharacter(CharacterDesc characterDesc)
	:m_CharacterDesc{ characterDesc }
{
	const auto material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	material->SetDiffuseTexture(L"../Resources/Textures/Chicken.png");

	m_pCharachter = new GameObject();
	auto model = m_pCharachter->AddComponent(new ModelComponent(L"../Resources/Meshes/CrossyRoad/Chicken.ovm"));
	model->SetMaterial(material);

	m_pCharachter->GetTransform()->Scale(1.f);
	AddChild(m_pCharachter);
	m_JumpTimer = m_JumpTime;
}

void CrossyCharacter::SetTerrain(TerrainGenerator* pTerrainGenerator)
{
	m_pTerrainGenerator = pTerrainGenerator;
}

void CrossyCharacter::Respawn()
{
	m_IsDead = false;
	m_StepsTaken = 0;
	m_TargetPos = {0,0};
	m_PrevPos = {0,0};
	m_pCharachter->GetTransform()->Translate(0,0,0);
	m_pCharachter->GetTransform()->Rotate(0, 0, 0);
}

void CrossyCharacter::Initialize(const SceneContext& sceneContext)
{
	m_StepsTaken = 0;
	GameObject::Initialize(sceneContext);
}

void CrossyCharacter::Update(const SceneContext& sceneContext)
{
	if (m_IsDead) return;
	
	//check if you just jumped into the water
	WaterSlice* pWater;
	if(m_pTerrainGenerator)
	{
		pWater = dynamic_cast<WaterSlice*>(m_pTerrainGenerator->GetCurrentTerrain(int(m_StepsTaken)));
		if (pWater)
		{
			if(!pWater->IsLilyPad(int(m_PrevPos.x/10)))
			{
				m_IsDead = true;
			}
		}
	}

	UpdateMovement(sceneContext);

	//implement Squish

	MoveCharacter();
	RotateCharacter(sceneContext);

}

void CrossyCharacter::DrawImGui()
{
	//ImGui::Begin("Character");
	//ImGui::End();
}

void CrossyCharacter::UpdateMovement(const SceneContext& sceneContext)
{

	bool isMoving = false;

	if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveForward)
		|| sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveBackward)
		|| sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveLeft)
		|| sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveRight))
		isMoving = true;

	if (m_JumpTimer <= 0.f)
	{
		m_PrevPos = m_TargetPos;

		if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveRight))
		{
			if (m_pTerrainGenerator->IsCurrentSlicePassable(int(m_TargetPos.x/10) + 1, int(m_StepsTaken)))
			{
				m_TargetPos.x += m_TileSize;
				SetRotation(180.f);
			}
		}

		if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveLeft))
		{
			if (m_pTerrainGenerator->IsCurrentSlicePassable(int(m_TargetPos.x/10) - 1, int(m_StepsTaken)))
			{
				m_TargetPos.x -= m_TileSize;
				SetRotation(0.f);
			}
		}
		if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveBackward))
		{
			if (m_pTerrainGenerator->IsCurrentSlicePassable(int(m_TargetPos.x/10), int(m_StepsTaken-1)))
			{
				m_TargetPos.y -= m_TileSize;
				m_StepsTaken--;
				SetRotation(270.f);
			}
		}

		if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveForward))
		{
			if (m_pTerrainGenerator->IsCurrentSlicePassable(int(m_TargetPos.x/10), int(m_StepsTaken+1)))
			{
				m_TargetPos.y += m_TileSize;
				m_StepsTaken++;
				SetRotation(90.f);
			}
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
