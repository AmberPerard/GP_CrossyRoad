#pragma once
#include "Prefabs/CrossyRoad/CrossyCharacter.h"

class Car final : public GameObject
{
public:

	enum class carModel
	{
		Orange,
		Blue,
		Green,
		Yellow,
		Purple
	};
	Car(float speed, int direction);
	~Car() override;

	Car(const Car& other) = delete;
	Car(Car&& other) noexcept = delete;
	Car& operator=(const Car& other) = delete;
	Car& operator=(Car&& other) noexcept = delete;

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
	void OnCollision(GameObject* pTrigger, GameObject* pOther, PxTriggerAction action);
private:
	float m_Speed;
	int m_Direction;

	static UINT m_OrangeTextureID;
	static UINT m_YellowTextureID;
	static UINT m_PurpleTextureID;
	static UINT m_GreenTextureID;
	static UINT m_BlueTextureID;

	GameObject* m_pCarCollision;

	void InitializeTextures();
	void ChooseRandomCar();

	//Sound
	FMOD::Channel* m_pChannelDeath{};
	FMOD::Sound* m_pCarDeath{};
	FMOD::Channel* m_pChannelCarPass{};
	FMOD::Sound* m_pCarPassNoHorn{};
	bool m_PassPlayed{ false };

	bool m_isTriggered{ false };
};



