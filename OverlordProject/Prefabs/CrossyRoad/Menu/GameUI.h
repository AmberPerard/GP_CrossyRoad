#pragma once
class GameUI final : public GameObject
{
public:
	GameUI() = default;
	~GameUI() override = default;

	GameUI(const GameUI& other) = delete;
	GameUI(GameUI&& other) noexcept = delete;
	GameUI& operator=(const GameUI& other) = delete;
	GameUI& operator=(GameUI&& other) noexcept = delete;

	void SetScore(int score);
	void SetGameOver(bool gameOver) { m_GameOver = gameOver; }
	void DrawImGui();
protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& sceneContext) override;

private:
	int m_Score{0};
	bool m_GameOver{ false };
	SpriteFont* m_pSmallFont{};
	SpriteFont* m_pBigFont{};

	bool m_ScoreTextDirty{ false };
	std::wstring m_ScoreText{};

	XMFLOAT3 m_Position{};

	const std::wstring m_GameOverText{ L"GAME OVER" };
	const std::wstring m_RestartText{ L"Press space to restart" };
};

