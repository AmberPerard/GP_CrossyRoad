#include "stdafx.h"
#include "CrossyCharacter.h"
#include "TerrainGenerator.h"
#include "Env/WaterSlice.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"

CrossyCharacter::CrossyCharacter(CharacterDesc characterDesc)
	:m_CharacterDesc{ characterDesc }
{
}

void CrossyCharacter::SetTerrain(TerrainGenerator* pTerrainGenerator)
{
	m_pTerrainGenerator = pTerrainGenerator;
}

void CrossyCharacter::SetDead(bool isDead)
{
	m_IsDead = isDead;
	m_pFeathers->SpawnOneShot();
}

void CrossyCharacter::Respawn()
{
	m_IsDead = false;
	m_StepsTaken = 0;
	m_TargetPos = { 0,0 };
	m_PrevPos = { 0,0 };
	m_pCharachter->GetTransform()->Translate(0, 0, 0);
	m_pCharachter->GetTransform()->Rotate(0, 0, 0);
}

void CrossyCharacter::Initialize(const SceneContext&)
{
	//Controller
	m_pControllerComponent = AddComponent(new ControllerComponent(m_CharacterDesc.controller));

	const auto material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	material->SetDiffuseTexture(L"../Resources/Textures/Chicken.png");

	m_pCharachter = new GameObject();
	auto model = m_pCharachter->AddComponent(new ModelComponent(L"../Resources/Meshes/CrossyRoad/Chicken.ovm"));
	model->SetMaterial(material);
	m_pCharachter->GetTransform()->Scale(10.f);

	m_JumpTimer = m_JumpTime;
	m_StepsTaken = 0;

	//sound
	FMOD::System* pFmod = SoundManager::Get()->GetSystem();
	pFmod->createStream("Resources/Audio/jump.wav", FMOD_DEFAULT, nullptr, &m_pJumpSound);
	pFmod->createStream("Resources/Audio/watersplashlow.mp3", FMOD_DEFAULT, nullptr, &m_pSplashSound);

	InitializeParticles();

	AddChild(m_pCharachter);
}

void CrossyCharacter::Update(const SceneContext& sceneContext)
{
	if (m_IsDead) {
		return;
	}
	//check if you just jumped into the water
	WaterSlice* pWater;
	if (m_pTerrainGenerator)
	{
		pWater = dynamic_cast<WaterSlice*>(m_pTerrainGenerator->GetCurrentTerrain(int(m_StepsTaken)));
		if (pWater)
		{
			if (!pWater->IsLilyPad(int(m_PrevPos.x / 10)))
			{
				m_pWaterSplash->SpawnOneShot();
				SoundManager::Get()->GetSystem()->playSound(m_pSplashSound, nullptr, false, &m_pChannelSplash);
				m_pChannelSplash->setVolume(0.6f);
				m_IsDead = true;

			}
		}
	}

	UpdateMovement(sceneContext);

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
			if (m_pTerrainGenerator->IsCurrentSlicePassable(int(m_TargetPos.x / 10) + 1, int(m_StepsTaken)))
			{
				if ((m_TargetPos.x + m_TileSize) <= (m_MaxStepsSideWays * m_TileSize))
				{
					m_TargetPos.x += m_TileSize;
					SetRotation(180.f);
				}
			}
		}

		if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveLeft))
		{
			if (m_pTerrainGenerator->IsCurrentSlicePassable(int(m_TargetPos.x / 10) - 1, int(m_StepsTaken)))
			{
				if ((m_TargetPos.x - m_TileSize) >= -(m_MaxStepsSideWays * m_TileSize))
				{
					m_TargetPos.x -= m_TileSize;
					SetRotation(0.f);
				}
			}
		}
		if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveBackward))
		{
			if (m_pTerrainGenerator->IsCurrentSlicePassable(int(m_TargetPos.x / 10), int(m_StepsTaken - 1)))
			{
				if (m_StepsTakenBackwards <= m_MaxStepsBackwards)
				{
					m_StepsTakenBackwards++;
					m_StepsTaken--;
					m_TargetPos.y -= m_TileSize;
					SetRotation(270.f);
				}
				else
				{
					m_IsDead = true;
				}
			}
		}

		if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveForward))
		{
			if (m_pTerrainGenerator->IsCurrentSlicePassable(int(m_TargetPos.x / 10), int(m_StepsTaken + 1)))
			{
				m_StepsTakenBackwards = 0;
				m_TargetPos.y += m_TileSize;
				m_StepsTaken++;
				SetRotation(90.f);
			}
		}

		if (isMoving)
		{
			m_JumpTimer = m_JumpTime;
			SoundManager::Get()->GetSystem()->playSound(m_pJumpSound, nullptr, false, &m_pChannelJump);
			m_pChannelJump->setVolume(.3f);
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

	//GetTransform()->Translate(xPos, m_CurrentHeight, zPos);
	XMFLOAT3 displacement;
	XMStoreFloat3(&displacement, { xPos, m_CurrentHeight, zPos });
	m_pControllerComponent->Translate(displacement);
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

void CrossyCharacter::InitializeParticles()
{
	//particles
	m_pFeathersObject = AddChild(new GameObject());

	m_FeathersSettings.minEmitterRadius = 1.f;
	m_FeathersSettings.maxEmitterRadius = 10.f;
	m_FeathersSettings.minScale = 1.f;
	m_FeathersSettings.maxScale = 3.f;
	m_FeathersSettings.velocity = { 2.f, 4.f, 10.f };
	m_FeathersSettings.minSize = 1.0f;
	m_FeathersSettings.maxSize = 2.f;
	m_FeathersSettings.minEnergy = 0.2f;
	m_FeathersSettings.maxEnergy = 1.f;

	m_pFeathers = m_pFeathersObject->AddComponent(new ParticleEmitterComponent(L"../Resources/Textures/crossy/DeathObject.png", m_FeathersSettings, 100, true));


	m_pWaterSplashObject = AddChild(new GameObject());

	m_WaterSplashSettings.minEmitterRadius = 1.f;
	m_WaterSplashSettings.maxEmitterRadius = 10.f;
	m_WaterSplashSettings.minScale = 1.f;
	m_WaterSplashSettings.maxScale = 3.f;
	m_WaterSplashSettings.velocity = { 0.f, -4.0f, 0.f };
	m_WaterSplashSettings.minSize = 1.f;
	m_WaterSplashSettings.maxSize = 2.0f;
	m_WaterSplashSettings.minEnergy = 0.2f;
	m_WaterSplashSettings.maxEnergy = 1.f;

	m_pWaterSplash = m_pWaterSplashObject->AddComponent(new ParticleEmitterComponent(L"../Resources/Textures/crossy/DeathWater.png", m_WaterSplashSettings, 100, true));

}
