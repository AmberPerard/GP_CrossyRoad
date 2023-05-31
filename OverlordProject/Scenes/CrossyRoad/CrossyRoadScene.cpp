#include "stdafx.h"
#include "CrossyRoadScene.h"

void CrossyRoadScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.drawGrid = false;

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

	//Camera
	m_pCamera = new FollowCamera(m_pCharacter,m_Pitch,m_Yaw,m_Distance);
	AddChild(m_pCamera);
	SetActiveCamera(m_pCamera->GetComponent<CameraComponent>());

	AddChild(m_pTerrainGenerator);
	AddChild(m_pCharacter);

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

}

void CrossyRoadScene::OnGUI()
{
	GameScene::OnGUI();
	m_pCharacter->DrawImGui();
	m_pCamera->DrawImGui();
}
