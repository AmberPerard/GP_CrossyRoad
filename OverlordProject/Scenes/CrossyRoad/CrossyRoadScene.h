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
	void Update() override;
	void PostDraw() override;

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
	FreeCamera* m_pFreeCamera{};
	float m_Pitch{45.f}, m_Yaw{-15.f}, m_Distance{500.f};

	XMFLOAT3 m_LightPos{ -40.f, 37.f, 15.f };
	XMFLOAT3 m_LightDirection{ 0.5298129428260175f, -0.6622661785325219f, 0.5298129428260175f };


	//Game over
	bool m_IsGameOver{ false };
};
