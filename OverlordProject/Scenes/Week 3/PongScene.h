#pragma once

class CubePrefab;
class SpherePrefab;

class PongScene final : public GameScene
{
public:
	PongScene() : GameScene(L"PongScene") {}
	~PongScene() override;
	PongScene(const PongScene&) = delete;
	PongScene(PongScene&&) noexcept = delete;
	PongScene& operator=(const PongScene&) = delete;
	PongScene& operator=(PongScene&&) noexcept = delete;

	enum PlayerStates
	{
		PLAYER1_UP,
		PLAYER1_DOWN,
		PLAYER2_UP,
		PLAYER2_DOWN,
		LAUNCH_BALL,
	};

protected:
	void Initialize() override;
	void Update() override;
	void OnGUI() override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;
private:
	// ball
	SpherePrefab* m_pSphere = nullptr;
	RigidBodyComponent* m_pSphereRigid = nullptr;

	// players
	CubePrefab* m_pPlayer1 = nullptr;
	CubePrefab* m_pPlayer2 = nullptr;

	float m_MovementSpeed = 0.20f;
	float m_Constraint = 8.5f;

	float m_OutOfBoundsDistance = 20.f;
	float m_MaxSpeedBall = 10.f;
	bool m_IsMoving = false;

	void MovePlayer(TransformComponent* pTransform, float speed) const;
	void ResetBall();
	void CreateRoom();
	void CreateStart();
	void LaunchBall();
};
