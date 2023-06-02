#pragma once
class Train final : public GameObject
{
public:
	Train(float speed, int direction);
	~Train() override;

	Train(const Train& other) = delete;
	Train(Train&& other) noexcept = delete;
	Train& operator=(const Train& other) = delete;
	Train& operator=(Train&& other) noexcept = delete;

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
private:
	float m_Speed;
	int m_Direction;

	GameObject* m_pCarCollision;
};


