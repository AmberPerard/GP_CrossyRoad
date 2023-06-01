#pragma once
class FollowCamera final : public GameObject
{
public:
	FollowCamera(GameObject* objToFollow, float pitch, float yaw, float distance);
	~FollowCamera() override = default;
	FollowCamera(const FollowCamera& other) = delete;
	FollowCamera(FollowCamera&& other) noexcept = delete;
	FollowCamera& operator=(const FollowCamera& other) = delete;
	FollowCamera& operator=(FollowCamera&& other) noexcept = delete;

	void SetRotation(float pitch, float yaw);
	void Reset();

	void DrawImGui();
	bool m_DisableFollow{ false };

protected:

	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& sceneContext) override;

private:

	float m_TotalPitch{}, m_TotalYaw{};
	float m_MoveSpeed{ 10.f }, m_RotationSpeed{ 10.f }, m_SpeedMultiplier{ 10.f };
	CameraComponent* m_pCamera{};
	GameObject* m_pTarget{};
	XMFLOAT3 m_StartPos;

	float m_Distance{ 10.f };
};

