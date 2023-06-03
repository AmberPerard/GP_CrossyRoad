#include "stdafx.h"
#include "CrossyRoadScene.h"

#include "Prefabs/CrossyRoad/Menu/Button.h"

void CrossyRoadScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.drawUserDebug = false;

	//Ground Plane
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMaterial);

	//Character
	CharacterDesc characterDesc{ pDefaultMaterial };
	characterDesc.actionId_MoveForward = CharacterMoveForward;
	characterDesc.actionId_MoveBackward = CharacterMoveBackward;
	characterDesc.actionId_MoveLeft = CharacterMoveLeft;
	characterDesc.actionId_MoveRight = CharacterMoveRight;

	m_pCharacter = new CrossyCharacter(characterDesc);
	m_pTerrainGenerator = new TerrainGenerator(m_pCharacter, 16, 4);
	m_pCharacter->SetTerrain(m_pTerrainGenerator);
	m_pUiObject = AddChild(new GameUI());

	//Camera
	m_pCamera = new FollowCamera(m_pCharacter, m_Pitch, m_Yaw, m_Distance);
	AddChild(m_pCamera);
	SetActiveCamera(m_pCamera->GetComponent<CameraComponent>());

	AddChild(m_pTerrainGenerator);
	AddChild(m_pCharacter);

	m_pTerrainGenerator->GetTransform()->Scale(10);

	//Input Actions
	auto inputAction = InputAction(CharacterMoveForward, InputState::pressed, VK_UP);
	m_SceneContext.pInput->AddInputAction(inputAction);
	inputAction = InputAction(CharacterMoveBackward, InputState::pressed, VK_DOWN);
	m_SceneContext.pInput->AddInputAction(inputAction);
	inputAction = InputAction(CharacterMoveLeft, InputState::pressed, VK_LEFT);
	m_SceneContext.pInput->AddInputAction(inputAction);
	inputAction = InputAction(CharacterMoveRight, InputState::pressed, VK_RIGHT);
	m_SceneContext.pInput->AddInputAction(inputAction);
	inputAction = InputAction(Restart, InputState::pressed, VK_SPACE);
	m_SceneContext.pInput->AddInputAction(inputAction);

	m_SceneContext.pLights->SetDirectionalLight({ -5.f, 5.f, m_pCharacter->GetPrevPosition().y - 2.1f }, m_LightDirection);

	//Post Processing
	m_pVignette = MaterialManager::Get()->CreateMaterial<PostVignette>();
	AddPostProcessingEffect(m_pVignette);
}

void CrossyRoadScene::OnGUI()
{
	//::SliderFloat2("button pos", &m_ButtonFloat_Continue.x, 0, 1000.f);
}

void CrossyRoadScene::Update()
{
	m_SceneContext.pLights->SetDirectionalLight({ m_LightPos.x, m_LightPos.y,  m_pCharacter->GetPrevPosition().y - m_LightPos.z }, m_LightDirection);
	int Score = m_pCharacter->GetStepsTaken();
	m_pUiObject->SetScore(Score);
	m_pUiObject->SetGameOver(m_IsGameOver);

	if ((m_IsGameOver && m_SceneContext.pInput->IsActionTriggered(Restart)) || (m_IsGameOver && m_IsStarted))
	{
		m_IsStarted = false;
		//spawn new terrain
		m_pTerrainGenerator->Reset();

		//reset player character & camera
		m_pCharacter->Respawn();
		if (m_pCamera) m_pCamera->Reset();

		//reset game over bool
		m_IsGameOver = false;
	}


	if (GetSceneContext().pInput->IsKeyboardKey(InputState::pressed, VK_ESCAPE))
	{
		if (m_IsPaused)
		{
			RemovePauseMenu();
			m_IsPaused = false;
			m_SceneContext.pGameTime->Start();
		}
		else
		{
			CreatePauseMenu();
			m_IsPaused = true;
			m_SceneContext.pGameTime->Stop();
		}
		if (!m_IsPaused && m_pPauseMenu != nullptr)
		{
			RemovePauseMenu();
		}
	}

	if (m_pCharacter->IsDead() && !m_IsGameOver)
	{
		//hide the player under the scene
		m_pCharacter->GetTransform()->Translate(0, -2.f, 0);
		if (m_pCamera) m_pCamera->m_DisableFollow = true;
		m_IsGameOver = true;
	}
}

void CrossyRoadScene::CreatePauseMenu()
{
	m_pPauseMenu = AddChild(new GameObject());
	m_pPauseMenu->AddComponent(new SpriteComponent(L"../Resources/Textures/crossy/GP2_Exam2023_PauseMenu.png", { 0, 1 }));
	m_pPauseMenu->GetTransform()->Translate(m_SceneContext.windowWidth / 2 - 200.f, m_SceneContext.windowHeight - 100.f, 0.1f);
	m_pPauseMenu->GetTransform()->Scale(0.8f, 0.8f, 0.8f);

	auto button = m_pPauseMenu->AddChild(new Button(L"Main Menu", m_ButtonFloat_Continue, XMFLOAT4{ Colors::White }, { 200.f,50.f },{60,10}));
	button->SetOnClickFunction([&]()
		{
			Reset();
			RemovePauseMenu();	
			SceneManager::Get()->SetActiveGameScene(L"CrossyMenuScene");
		});

	auto secondButton = m_pPauseMenu->AddChild(new Button(L"Reset", m_ButtonFloat_Reset, XMFLOAT4{ Colors::White }, { 200.f,50.f },{10,10}));
	secondButton->SetOnClickFunction([&]()
		{
			Reset();
			RemovePauseMenu();
		});

	auto thirdButton = m_pPauseMenu->AddChild(new Button(L"Quit", m_ButtonFloat_Quit, XMFLOAT4{ Colors::White }, { 200.f,50.f }, {-5,10}));
	thirdButton->SetOnClickFunction([&]()
		{
			PostQuitMessage(0);
		});

}

void CrossyRoadScene::RemovePauseMenu()
{
	if (m_pPauseMenu)
	{
		for (auto& child : m_pPauseMenu->GetChildren<GameObject>())
		{
			m_pPauseMenu->RemoveChild(child, true);
		}

		RemoveChild(m_pPauseMenu, true);

		m_pPauseMenu = nullptr;
	}
}

void CrossyRoadScene::Reset()
{
	m_pTerrainGenerator->Reset();
	m_pCharacter->Respawn();
	if (m_pCamera) m_pCamera->Reset();

	m_SceneContext.pGameTime->Start();
	m_IsGameOver = false;
	m_IsPaused = false;

	m_pUiObject->SetGameOver(m_IsGameOver);
	m_pUiObject->SetScore(0);
}
