#include "stdafx.h"
#include "Button.h"

Button::Button(std::wstring text, XMFLOAT2 position, XMFLOAT4 color, XMFLOAT2 bounds, XMFLOAT2 textOffset)
	: m_Text{ text },
	m_Position{ position },
	m_Color{ color },
	m_Bounds{ bounds },
	m_TextPosition{ textOffset }
{
	m_pFont = ContentManager::Load<SpriteFont>(L"../Resources/SpriteFonts/CrossyRoad.fnt");
}

void Button::SetOnClickFunction(std::function<void()> newClickFunction)
{
	m_OnClickFunction = newClickFunction;
}

void Button::Initialize(const SceneContext&)
{
	auto sprite = new SpriteComponent(L"../Resources/Textures/crossy/GP2_Exam2023_Menu_Button.png", XMFLOAT2{ 0.5f, 0.5f });
	AddComponent(sprite);
	sprite->GetTransform()->Translate(m_Position.x + 64, m_Position.y + 25.f, 0.01f);
	sprite->GetTransform()->Scale(1.5f, 0.6f, 1);
}

void Button::Draw(const SceneContext& sceneContext)
{
	DebugRenderer::DrawLine(XMFLOAT3{ m_Position.x, m_Position.y, 0.f }, XMFLOAT3{ m_Position.x + m_Bounds.x, m_Position.y ,0 });

	auto textPos = XMFLOAT2{ m_Position.x - m_TextPosition.x, m_Position.y + m_TextPosition.y };
	if (!IsOverrlapping(sceneContext))
	{
		TextRenderer::Get()->DrawText(m_pFont, m_Text, textPos, m_Color);
		return;
	}

	TextRenderer::Get()->DrawText(m_pFont, m_Text, textPos, XMFLOAT4{ Colors::Black });
	if (sceneContext.pInput->IsMouseButton(InputState::pressed, VK_LBUTTON) && m_OnClickFunction != nullptr)
	{
		m_OnClickFunction();
	}
}

bool Button::IsOverrlapping(const SceneContext& sceneContext)
{
	auto mousePos = sceneContext.pInput->GetMousePosition();

	if (mousePos.x > m_Position.x - 64 && mousePos.x < m_Position.x + m_Bounds.x
		&& mousePos.y > m_Position.y && mousePos.y < m_Position.y + m_Bounds.y)
	{
		return true;
	}

	return false;
}
