#pragma once
class Obstacle : public GameObject
{
public:
	Obstacle() = default;
	~Obstacle() = default;

	Obstacle(const Obstacle& other) = delete;
	Obstacle(Obstacle&& other) noexcept = delete;
	Obstacle& operator=(const Obstacle& other) = delete;
	Obstacle& operator=(Obstacle&& other) noexcept = delete;

protected:
	void Initialize(const SceneContext&) override = 0;
	GameObject* m_pObject{};
};

