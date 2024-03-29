#pragma once
#include "Prefabs/Character.h"

class TerrainGenerator;

class CrossyCharacter final : public GameObject
{
public:
	CrossyCharacter(CharacterDesc characterDesc);
	~CrossyCharacter() override = default;

	CrossyCharacter(const CrossyCharacter& other) = delete;
	CrossyCharacter(CrossyCharacter&& other) noexcept = delete;
	CrossyCharacter& operator=(const CrossyCharacter& other) = delete;
	CrossyCharacter& operator=(CrossyCharacter&& other) noexcept = delete;

	void DrawImGui();
	XMFLOAT2 GetPrevPosition() const { return m_PrevPos; };
	int GetStepsTaken() const { return m_StepsTaken; };
	void SetTerrain(TerrainGenerator* pTerrainGenerator);
	bool IsDead() const { return m_IsDead; };
	void SetDead(bool isDead);;

	void Respawn();
protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
private:
	GameObject* m_pCharachter = nullptr;
	ControllerComponent* m_pControllerComponent{};
	CharacterDesc m_CharacterDesc;

	//Movement
	XMFLOAT2 m_TargetPos{ 0,0 };
	XMFLOAT2 m_PrevPos{0,0};

	const float m_TileSize{ 10 };

	int m_StepsTaken{0};
	const int m_MaxStepsBackwards{ 2 };
	const int m_MaxStepsSideWays{ 4 };
	int m_StepsTakenBackwards{};

	const float m_JumpTime{ 0.1f };
	float m_JumpTimer{ 0.f };
	const float m_JumpHeight{ 10.f };
	float m_CurrentHeight{0};

	TerrainGenerator* m_pTerrainGenerator{};
	void UpdateMovement(const SceneContext& sceneContext);
	void MoveCharacter();

	//Squish
	float m_SquishFactor{ 0 };
	float m_MaxSquishScale{ 2.f };

	//Rotation
	float m_PrevRotationY{};
	float m_TargetRotationY{};
	float m_RotationTimer{};
	const float m_RotationTime{ 0.08f };

	void RotateCharacter(const SceneContext& sceneContext);
	void SetRotation(float rotation);

	//Charachter life
	bool m_IsDead{ false };

	//Sound
	FMOD::Channel* m_pChannelJump{};
	FMOD::Sound* m_pJumpSound{};
	FMOD::Channel* m_pChannelSplash{};
	FMOD::Sound* m_pSplashSound{};

	//Particles
	GameObject* m_pFeathersObject = nullptr;
	ParticleEmitterSettings m_FeathersSettings{};
	ParticleEmitterComponent* m_pFeathers = nullptr;

	GameObject* m_pWaterSplashObject = nullptr;
	ParticleEmitterSettings m_WaterSplashSettings{};
	ParticleEmitterComponent* m_pWaterSplash = nullptr;
	void InitializeParticles();
};

