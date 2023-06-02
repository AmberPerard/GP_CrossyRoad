#pragma once
class Car final : public GameObject
{
public:
	Car(float speed, int direction);
	~Car() override;

	Car(const Car& other) = delete;
	Car(Car&& other) noexcept = delete;
	Car& operator=(const Car& other) = delete;
	Car& operator=(Car&& other) noexcept = delete;

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
private:
	float m_Speed;
	int m_Direction;

	GameObject* m_pCarCollision;
};

