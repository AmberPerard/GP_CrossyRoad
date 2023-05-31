#pragma once
#include "Prefabs/FollowCamera.h"
#include "Prefabs/CrossyRoad/CrossyCharacter.h"
#include "Prefabs/CrossyRoad/TerrainGenerator.h"

class CrossyRoadScene : public GameScene
{
public:
	CrossyRoadScene() :GameScene(L"CrossyRoadScene") {}
	~CrossyRoadScene() override = default;
	CrossyRoadScene(const CrossyRoadScene& other) = delete;
	CrossyRoadScene(CrossyRoadScene&& other) noexcept = delete;
	CrossyRoadScene& operator=(const CrossyRoadScene& other) = delete;
	CrossyRoadScene& operator=(CrossyRoadScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void OnGUI() override;

private:
	enum InputIds
	{
		CharacterMoveLeft,
		CharacterMoveRight,
		CharacterMoveForward,
		CharacterMoveBackward,
		Restart
	};

	CrossyCharacter* m_pCharacter{};
	TerrainGenerator* m_pTerrainGenerator{};
	FollowCamera* m_pCamera{};
	float m_Pitch{50.f}, m_Yaw{-15.f}, m_Distance{10.f};
};
