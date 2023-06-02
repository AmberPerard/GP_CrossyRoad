#pragma once
class Train final : public GameObject
{
public:
	enum class TrainPiece
	{
		front,
		middle,
		end
	};

	Train(float speed, int direction, float xPos, TrainPiece whatTrainPiece);
	~Train() override;

	Train(const Train& other) = delete;
	Train(Train&& other) noexcept = delete;
	Train& operator=(const Train& other) = delete;
	Train& operator=(Train&& other) noexcept = delete;

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
	void OnCollision(GameObject* pTrigger, GameObject* pOther, PxTriggerAction action);
private:
	float m_Speed;
	int m_Direction;
	float m_xPos;

	static UINT m_TrainFrontTextureID;
	static UINT m_TrainMiddleTextureID;
	static UINT m_TrainEndTextureID;

	TrainPiece m_WhatTrainPiece;
	GameObject* m_pTrainCollision;

	void InitializeTextures();
	void ChooseCorrectTrainPiece();

	//Sound
	FMOD::Channel* m_pChannelDeathTrain{};
	FMOD::Sound* m_pTrainDeath{};
	FMOD::Channel* m_pChannelAlarm{};
	FMOD::Sound* m_pTrainAlarm{};
	FMOD::Channel* m_pChannelTrainPass{};
	FMOD::Sound* m_pTrainPassNoHorn{};
	FMOD::Sound* m_pTrainPassHorn{};
	bool m_PassPlayed{ false };
};


