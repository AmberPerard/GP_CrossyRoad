#include "stdafx.h"
#include "CrossyMenuScene.h"
#include "Materials/DiffuseMaterial_Skinned.h"
#include "Prefabs/CrossyRoad/Menu/Button.h"

void CrossyMenuScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.drawUserDebug = false;

	//CAMERA
	m_pCamera = new FixedCamera();
	AddChild(m_pCamera);
	CameraComponent* pCamComp = m_pCamera->GetComponent<CameraComponent>();
	pCamComp->SetOrthoSize(8.f);
	m_pCamera->GetTransform()->Rotate(m_Pitch, m_Yaw, 0.f);
	m_pCamera->GetTransform()->Translate(m_CameraPos);
	SetActiveCamera(pCamComp);

	SpawnBackgroundSprites();
	SpawnWolfsObjects();

	m_pMenuButtons = new GameObject();
	auto button = m_pMenuButtons->AddChild(new Button(L"Start", m_ButtonFloatStart, XMFLOAT4{ Colors::White }, { 200.f,50.f },{5,10}));
	button->SetOnClickFunction([&]()
		{
			//start game
			SceneManager::Get()->NextScene();
		});

	auto secondButton = m_pMenuButtons->AddChild(new Button(L"Quit", m_ButtonFloatQuit, XMFLOAT4{ Colors::White }, { 200.f,50.f },{0,10}));
	secondButton->SetOnClickFunction([&]()
		{
			PostQuitMessage(0);
		});
	AddChild(m_pMenuButtons);
}

void CrossyMenuScene::OnGUI()
{
}

void CrossyMenuScene::Update()
{
	GameObject* picked = m_SceneContext.pCamera->Pick();
	if (picked)
	{
		m_StartedHovered = picked->GetTag() == L"Start";
		m_QuitHovered = picked->GetTag() == L"Quit";
	}
	else
	{
		m_QuitHovered = false;
		m_StartedHovered = false;
	}


	if (m_StartedHovered && pAnimatorStart->GetClipName() != L"dancing")
	{
		pAnimatorStart->SetAnimation(L"dancing");
		pAnimatorStart->Play();
	}
	else if (!m_StartedHovered && pAnimatorStart->GetClipName() != L"Idle")
	{
		pAnimatorStart->SetAnimation(L"Idle");
		pAnimatorStart->Play();
	}

	if (m_QuitHovered && pAnimatorQuit->GetClipName() != L"Wave")
	{
		pAnimatorQuit->SetAnimation(L"Wave");
		pAnimatorQuit->Play();
	}
	else if (!m_QuitHovered && pAnimatorQuit->GetClipName() != L"Idle")
	{
		pAnimatorQuit->SetAnimation(L"Idle");
		pAnimatorQuit->Play();
	}

	if (m_SceneContext.pInput->IsMouseButton(InputState::down, VK_LBUTTON))
	{
		if (m_StartedHovered)
		{
			//Start Game
			SceneManager::Get()->NextScene();
		}

		if (m_QuitHovered)
		{
			PostQuitMessage(0);
		}
	}
}

void CrossyMenuScene::SpawnBackgroundSprites()
{
	m_pBackground = AddChild(new GameObject());
	m_pBackground->AddComponent(new SpriteComponent(L"../Resources/Textures/crossy/GP2_Exam2023_MainMenu_Background.png", { 0, 1 }));
	m_pBackground->GetTransform()->Translate(0.f, m_SceneContext.windowHeight, 0.990f);
	m_pBackground->GetTransform()->Scale(1.01f, 1.f, 1.0f);
}

void CrossyMenuScene::SpawnWolfsObjects()
{
	//Start button
	m_pStartObject = AddChild(new GameObject());
	m_pStartObject->SetTag(L"Start");

	//set model + material
	auto modComp = m_pStartObject->AddComponent(new ModelComponent(L"../Resources/Meshes/CrossyRoad/Character/Wolf2.ovm"));
	auto mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Skinned>();
	mat->SetDiffuseTexture(L"../Resources/Textures/crossy/WolfTexture.png");
	modComp->SetMaterial(mat);

	//create collider to click on
	auto rigi = m_pStartObject->AddComponent(new RigidBodyComponent(true));
	PxMaterial* physMat = PxGetPhysics().createMaterial(0.f, 0.f, 0.f);
	const auto pMesh = ContentManager::Load<PxConvexMesh>(L"../Resources/Meshes/CrossyRoad/Character/Wolf2.ovpc");
	rigi->AddCollider(PxConvexMeshGeometry(pMesh, PxMeshScale(0.04f)), *physMat, false);

	//set animator
	pAnimatorStart = modComp->GetAnimator();
	pAnimatorStart->SetAnimation(L"Idle");
	pAnimatorStart->Play();

	//set transform
	m_pStartObject->GetTransform()->Scale(0.04f);
	m_pStartObject->GetTransform()->Translate(-2.f, -2.f, -0.990f);



	//Quit button
	m_pQuitObject = AddChild(new GameObject());
	m_pQuitObject->SetTag(L"Quit");

	//set model + material
	auto modCompQuit = m_pQuitObject->AddComponent(new ModelComponent(L"../Resources/Meshes/CrossyRoad/Character/Wolf2.ovm"));
	modCompQuit->SetMaterial(mat);

	//create collider to click on
	rigi = m_pQuitObject->AddComponent(new RigidBodyComponent(true));
	rigi->AddCollider(PxConvexMeshGeometry(pMesh, PxMeshScale(0.04f)), *physMat, false);

	//set animator
	pAnimatorQuit = modCompQuit->GetAnimator();
	pAnimatorQuit->SetAnimation(L"Idle");
	pAnimatorQuit->Play();

	//set transform
	m_pQuitObject->GetTransform()->Scale(0.04f);
	m_pQuitObject->GetTransform()->Translate(2.f, -2.f, -0.990f);
}
