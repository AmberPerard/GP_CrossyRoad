#pragma once
#include "Prefabs/Character.h"

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

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
private:
	GameObject* m_pCharachter;
	CharacterDesc m_CharacterDesc;

	//Movement
	XMFLOAT2 m_TargetPos{};
	XMFLOAT2 m_PrevPos{};

	const float m_TileSize{ 1 };

	const float m_JumpTime{ 0.1f };
	float m_JumpTimer{ 0.f };
	const float m_JumpHeight{ 1.f };
	float m_CurrentHeight{0};

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
};

