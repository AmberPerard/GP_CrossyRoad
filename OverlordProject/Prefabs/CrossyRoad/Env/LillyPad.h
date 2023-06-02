#pragma once
class LillyPad final : public GameObject
{
	public:
	LillyPad() = default;
	~LillyPad() = default;

	LillyPad(const LillyPad& other) = delete;
	LillyPad(LillyPad&& other) noexcept = delete;
	LillyPad& operator=(const LillyPad& other) = delete;
	LillyPad& operator=(LillyPad&& other) noexcept = delete;

	//Custom functions
	void Initialize(const SceneContext&) override;
private:
	static UINT m_LilyTextureID;
};

