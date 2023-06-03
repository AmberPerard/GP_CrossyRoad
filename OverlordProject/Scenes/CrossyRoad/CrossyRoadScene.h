#pragma once
#include "Materials/Post/PostVignette.h"
#include "Prefabs/FollowCamera.h"
#include "Prefabs/CrossyRoad/CrossyCharacter.h"
#include "Prefabs/CrossyRoad/TerrainGenerator.h"
#include "Prefabs/CrossyRoad/Menu/GameUI.h"

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
	bool m_IsStarted{ true };
	bool m_IsGameOver{ false };

	//InGame HUD
	GameUI* m_pUiObject = nullptr;

	//Pause menu
	bool m_IsPaused{ false };
	GameObject* m_pPauseMenu = nullptr;
	void CreatePauseMenu();
	void RemovePauseMenu();

	XMFLOAT2 m_ButtonFloat_Continue{570.f,365.f};
	XMFLOAT2 m_ButtonFloat_Reset{ 570.f,435.f };
	XMFLOAT2 m_ButtonFloat_Quit{ 570.f,515.f };
	void Reset();

	//post processing
	PostVignette* m_pVignette{};
};
