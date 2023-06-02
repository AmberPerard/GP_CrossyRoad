#include "stdafx.h"
#include "CrossyRoadScene.h"

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

	//Camera
	//m_pFreeCamera = new FreeCamera();
	m_pCamera = new FollowCamera(m_pCharacter, m_Pitch, m_Yaw, m_Distance);
	AddChild(m_pCamera);
	SetActiveCamera(m_pCamera->GetComponent<CameraComponent>());

	AddChild(m_pTerrainGenerator);
	AddChild(m_pCharacter);

	m_pCharacter->GetTransform()->Scale(10);
	m_pTerrainGenerator->GetTransform()->Scale(10);

	//input actions
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

	//m_SceneContext.pLights->SetDirectionalLight({ -95.6139526f,66.1346436f,-41.1850471f }, { 0.740129888f, -0.597205281f, 0.309117377f });
	m_SceneContext.pLights->SetDirectionalLight({ -5.f, 5.f, m_pCharacter->GetPrevPosition().y - 2.1f }, m_LightDirection);
}

void CrossyRoadScene::OnGUI()
{
	GameScene::OnGUI();
	m_pCharacter->DrawImGui();
	m_pCamera->DrawImGui();
	ImGui::Begin("lights");

	ImGui::Text("LightDir");
	ImGui::SliderFloat("lightDir x", &m_LightDirection.x, -1.f, 1.f);
	ImGui::SliderFloat("lightDir y", &m_LightDirection.y, -1.f, 1.f);
	ImGui::SliderFloat("lightDir z", &m_LightDirection.z, -1.f, 1.f);

	ImGui::Text("LightPos");
	ImGui::SliderFloat("lightpos x", &m_LightPos.x, -100.f, 100.f);
	ImGui::SliderFloat("lightpos y", &m_LightPos.y, -100.f, 100.f);
	ImGui::SliderFloat("lightpos z", &m_LightPos.z, -100.f, 100.f);

	ImGui::End();

}

void CrossyRoadScene::Update()
{
	GameScene::Update();
	m_SceneContext.pLights->SetDirectionalLight({ m_LightPos.x, m_LightPos.y,  m_pCharacter->GetPrevPosition().y - m_LightPos.z }, m_LightDirection);
	if (m_pCharacter->IsDead() && !m_IsGameOver)
	{
		//hide the player under the scene
		m_pCharacter->GetTransform()->Translate(0, -2.f, 0);
		m_pCamera->m_DisableFollow = true;
		m_IsGameOver = true;
	}

	if (m_IsGameOver && m_SceneContext.pInput->IsActionTriggered(Restart))
	{
		//spawn new terrain
		m_pTerrainGenerator->Reset();

		//reset player character & camera
		m_pCharacter->Respawn();
		if (m_pCamera) m_pCamera->Reset();

		//reset game over bool
		m_IsGameOver = false;
	}
}