#include "stdafx.h"
#include "FollowCamera.h"

void FollowCamera::Initialize(const SceneContext& sceneContext)
{
	GameObject::Initialize(sceneContext);
	m_pCamera = new CameraComponent();
	AddComponent(m_pCamera);
	m_pCamera->UseOrthographicProjection();
	m_pCamera->SetOrthoSize(8.f);
	m_pCamera->SetFieldOfView(XMConvertToRadians(8.f));

	GetTransform()->Rotate(m_TotalPitch, m_TotalYaw, 0.f);

	//lookat translation
	XMFLOAT3 forward{ sin(XMConvertToRadians(m_TotalYaw)) * cos(XMConvertToRadians(m_TotalPitch)) ,
					sin(XMConvertToRadians(-m_TotalPitch)),
					cos(XMConvertToRadians(m_TotalYaw)) * cos(XMConvertToRadians(m_TotalPitch))
	};

	m_StartPos = XMFLOAT3(-forward.x * m_Distance, -forward.y * m_Distance, -forward.z * m_Distance + 2);
	GetTransform()->Translate(m_StartPos);

	//activate
	m_pCamera->SetActive(true);
}

void FollowCamera::Update(const SceneContext& sceneContext)
{
	GetTransform()->Rotate(m_TotalPitch, m_TotalYaw, 0.f);

	//lookat translation
	XMFLOAT3 forward{ sin(XMConvertToRadians(m_TotalYaw)) * cos(XMConvertToRadians(m_TotalPitch)) ,
					sin(XMConvertToRadians(-m_TotalPitch)),
					cos(XMConvertToRadians(m_TotalYaw)) * cos(XMConvertToRadians(m_TotalPitch))
	};

	m_StartPos = XMFLOAT3(-forward.x * m_Distance, -forward.y * m_Distance, -forward.z * m_Distance + 2);

	//follow the player in the z direction
	float value{ 2.f * sceneContext.pGameTime->GetElapsed() };
	MathHelper::Clamp(value, 1.f, 0.f);
	GetTransform()->Translate(m_StartPos.x, m_StartPos.y, m_StartPos.z + std::lerp((GetTransform()->GetPosition().z - m_StartPos.z), m_pTarget->GetTransform()->GetWorldPosition().z, value));
	GameObject::Update(sceneContext);

}

FollowCamera::FollowCamera(GameObject* objToFollow, float pitch, float yaw, float distance)
	:m_TotalPitch(pitch), m_TotalYaw(yaw), m_pTarget(objToFollow), m_Distance(distance)
{
}

void FollowCamera::SetRotation(float pitch, float yaw)
{
	m_TotalPitch = pitch;
	m_TotalYaw = yaw;
}

void FollowCamera::Reset()
{
	GetTransform()->Translate(m_StartPos);
}

void FollowCamera::DrawImGui()
{
	ImGui::Begin("Camera");
	ImGui::SliderFloat("Pitch", &m_TotalPitch, -90.f, 90.f);
	ImGui::SliderFloat("Yaw", &m_TotalYaw, -180.f, 180.f);
	ImGui::SliderFloat("Distance", &m_Distance, 1.f, 20.f);
	ImGui::End();
}

