#pragma once
class CrossyMenuScene : public GameScene
{
public:
	CrossyMenuScene() :GameScene(L"CrossyMenuScene") {}
	~CrossyMenuScene() override;
	CrossyMenuScene(const CrossyMenuScene& other) = delete;
	CrossyMenuScene(CrossyMenuScene&& other) noexcept = delete;
	CrossyMenuScene& operator=(const CrossyMenuScene& other) = delete;
	CrossyMenuScene& operator=(CrossyMenuScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void OnGUI() override;
	void Update() override;
private:
	//Camera
	FixedCamera* m_pCamera{};
	float m_Pitch{ 20.f };
	float m_Yaw{ 0.f };

	XMFLOAT3 m_CameraPos{ 0.f, 3.14f, -9.52f };

	//Buttons
	//start
	GameObject* m_pStartObject{};
	ModelAnimator* pAnimatorStart{};

	//quit
	GameObject* m_pQuitObject{};
	ModelAnimator* pAnimatorQuit{};

	//background
	GameObject* m_pBackground{};

	bool m_StartedHovered{ false };
	bool m_QuitHovered{ false };

	void SpawnBackgroundSprites();
	void SpawnButtonObjects();
};

