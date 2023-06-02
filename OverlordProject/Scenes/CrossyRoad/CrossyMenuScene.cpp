#include "stdafx.h"
#include "CrossyMenuScene.h"

#include "Materials/DiffuseMaterial_Skinned.h"

CrossyMenuScene::~CrossyMenuScene()
{
	for (UINT i{ 0 }; i < m_ClipCount; ++i)
	{
		delete[] m_ClipNames[i];
	}

	delete[] m_ClipNames;
}

void CrossyMenuScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;
	//CAMERA
	GameObject* pCam = AddChild(new GameObject());
	CameraComponent* pCamComp = pCam->AddComponent(new CameraComponent());
	//pCamComp->UseOrthographicProjection();
	pCamComp->SetOrthoSize(8.f);

	float distance = 10.f;
	float pitch{ 20.f };
	float yaw{ 0.f };
	pCam->GetTransform()->Rotate(pitch, yaw, 0.f);

	//lookat translation
	XMFLOAT3 forward{ sin(XMConvertToRadians(yaw)) * cos(XMConvertToRadians(pitch)) ,
					sin(XMConvertToRadians(-pitch)),
					cos(XMConvertToRadians(yaw)) * cos(XMConvertToRadians(pitch))
	};

	XMFLOAT3 transform = XMFLOAT3(-forward.x * distance, -forward.y * distance, -forward.z * distance);
	pCam->GetTransform()->Translate(transform);

	//activate
	pCamComp->SetActive(true);

	//pSpriteObject = AddChild(new GameObject());
	//pSpriteObject->AddComponent(new SpriteComponent(L"../Resources/Textures/crossy/GP2_Exam2023_MainMenu_Background.png", { 0, 1 }));
	//pSpriteObject->GetTransform()->Translate(0.f, m_SceneContext.windowHeight, 0.f);
	//pSpriteObject->GetTransform()->Scale(0.3f, 0.3f, 0.3f);

	m_pBackground = AddChild(new GameObject());
	m_pBackground->AddComponent(new SpriteComponent(L"../Resources/Textures/crossy/GP2_Exam2023_MainMenu_Background.png", { 0, 1 }));
	m_pBackground->GetTransform()->Translate(0.f, m_SceneContext.windowHeight, 0.990f);
	m_pBackground->GetTransform()->Scale(1.01f, 1.f, 1.0f);



	m_pStartButton = AddChild(new GameObject());
	m_pStartButton->SetTag(L"Start");

	auto modComp = m_pStartButton->AddComponent(new ModelComponent(L"../Resources/Meshes/CrossyRoad/Character/Wolf.ovm"));
	pAnimator = modComp->GetAnimator();
	pAnimator->SetAnimation(L"Idle");
	pAnimator->Play();
	auto mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Skinned>();
	mat->SetDiffuseTexture(L"../Resources/Textures/crossy/WolfTexture.png");
	modComp->SetMaterial(mat);


	auto rigi = m_pStartButton->AddComponent(new RigidBodyComponent(true));
	PxMaterial* physMat = PxGetPhysics().createMaterial(0.f, 0.f, 0.f);
	const auto pMesh = ContentManager::Load<PxConvexMesh>(L"../Resources/Meshes/CrossyRoad/Character/Wolf.ovpc");
	rigi->AddCollider(PxConvexMeshGeometry(pMesh, PxMeshScale(0.04f)), *physMat, false);

	m_pStartButton->GetTransform()->Scale(0.04f);
	m_pStartButton->GetTransform()->Translate(-2.f, -2.f, -0.990f);
}

void CrossyMenuScene::OnGUI()
{
	if (ImGui::Button(pAnimator->IsPlaying() ? "PAUSE" : "PLAY"))
	{
		if (pAnimator->IsPlaying())pAnimator->Pause();
		else pAnimator->Play();
	}

	if (ImGui::Button("RESET"))
	{
		pAnimator->Reset();
	}

	ImGui::Dummy({ 0,5 });

	bool reversed = pAnimator->IsReversed();
	if (ImGui::Checkbox("Play Reversed", &reversed))
	{
		pAnimator->SetPlayReversed(reversed);
	}

	if (ImGui::ListBox("Animation Clip", &m_AnimationClipId, m_ClipNames, static_cast<int>(m_ClipCount)))
	{
		pAnimator->SetAnimation(m_AnimationClipId);
	}

	if (ImGui::SliderFloat("Animation Speed", &m_AnimationSpeed, 0.f, 4.f))
	{
		pAnimator->SetAnimationSpeed(m_AnimationSpeed);
	}

	ImGui::SliderFloat("Z value wolf", &zValueWolf, -100.f, 100.f);
	ImGui::SliderFloat("Z value background", &zValuebackground, 0.f, 1.f);
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


	if (m_StartedHovered && pAnimator->GetClipName() != L"Wave")
	{
		pAnimator->SetAnimation(L"Wave");
		pAnimator->Play();
	}
	else if (!m_StartedHovered && pAnimator->GetClipName() != L"Idle")
	{
		pAnimator->SetAnimation(L"Idle");
		pAnimator->Play();
	}

	if (m_QuitHovered && pAnimator->GetClipName() != L"Wave")
	{
		pAnimator->SetAnimation(L"Wave");
		pAnimator->Play();
	}
	else if (!m_QuitHovered && pAnimator->GetClipName() != L"Idle")
	{
		pAnimator->SetAnimation(L"Idle");
		pAnimator->Play();
	}

	if (m_SceneContext.pInput->IsMouseButton(InputState::down, VK_LBUTTON))
	{
		if (m_StartedHovered)
		{
			//start game
			SceneManager::Get()->NextScene();
		}

		if (m_QuitHovered)
		{
			PostQuitMessage(0);
		}
	}
}
