#pragma once
class SpikeMaterial;

class SpikyScene final : public GameScene
{
public:
	SpikyScene() :GameScene(L"SpikyScene") {}
	~SpikyScene() override = default;

	SpikyScene(const SpikyScene& other) = delete;
	SpikyScene(SpikyScene&& other) noexcept = delete;
	SpikyScene& operator=(const SpikyScene& other) = delete;
	SpikyScene& operator=(SpikyScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void OnGUI() override;
private:
	SpikeMaterial* m_pMaterial = nullptr;
	GameObject* m_pObjects = nullptr;
};
