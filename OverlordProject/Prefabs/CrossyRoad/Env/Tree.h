#pragma once
#include "Obstacle.h"

class Tree : public Obstacle
{
public:

	enum TreeSize
	{
		Small,
		Tall
	};

	Tree() = default;
	~Tree() = default;

	Tree(const Tree& other) = delete;
	Tree(Tree&& other) noexcept = delete;
	Tree& operator=(const Tree& other) = delete;
	Tree& operator=(Tree&& other) noexcept = delete;
protected:
	void Initialize(const SceneContext&) override;
	UINT m_SmallTextureID;
	UINT m_TallTextureID;
};

