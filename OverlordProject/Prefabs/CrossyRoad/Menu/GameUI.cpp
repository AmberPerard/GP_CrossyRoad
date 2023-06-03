#include "stdafx.h"
#include "GameUI.h"

void GameUI::SetScore(int score)
{
	m_Score = score;
	m_ScoreTextDirty = true;
}

void GameUI::DrawImGui()
{
	ImGui::Begin("GameUI");
	ImGui::SliderFloat3("Position", &m_Position.x, -100.f, 100.f);
	ImGui::End();
}

void GameUI::Initialize(const SceneContext&)
{
	m_ScoreTextDirty = true;
	m_pSmallFont = ContentManager::Load<SpriteFont>(L"../Resources/SpriteFonts/CrossyRoad.fnt");
}

void GameUI::Update(const SceneContext& sceneContext)
{
	GetTransform()->Rotate(45.f, -15.f, 0.f);
	GetTransform()->Translate(m_Position);

	if (m_ScoreTextDirty)
	{
		m_ScoreText = L"SCORE " + std::to_wstring(m_Score);
		m_ScoreTextDirty = false;
	}

	if (m_GameOver)
	{
		TextRenderer::Get()->DrawText(m_pSmallFont, m_GameOverText, { sceneContext.windowWidth / 2 - 120.f, sceneContext.windowHeight / 2 - 50.f }, XMFLOAT4{ Colors::White });
		TextRenderer::Get()->DrawText(m_pSmallFont, m_RestartText, { sceneContext.windowWidth / 2 - 260.f, sceneContext.windowHeight / 2 + 20.f }, XMFLOAT4{ Colors::White });
	}

	TextRenderer::Get()->DrawText(m_pSmallFont, m_ScoreText, { sceneContext.windowWidth / 2 - 90.f, 20.f });
}
