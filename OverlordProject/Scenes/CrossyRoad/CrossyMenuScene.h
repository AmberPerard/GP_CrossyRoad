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
	ModelAnimator* pAnimator{};

	bool m_StartedHovered{ false };
	bool m_QuitHovered{ false };

	int m_AnimationClipId{ 0 };
	float m_AnimationSpeed{ 1.f };

	char** m_ClipNames{};
	UINT m_ClipCount{};
};

