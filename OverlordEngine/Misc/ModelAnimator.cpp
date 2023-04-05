#include "stdafx.h"
#include "ModelAnimator.h"

ModelAnimator::ModelAnimator(MeshFilter* pMeshFilter) :
	m_pMeshFilter{ pMeshFilter }
{
	SetAnimation(0);
}

void ModelAnimator::Update(const SceneContext& sceneContext)
{

	//We only update the transforms if the animation is running and the clip is set
	if (m_IsPlaying && m_ClipSet)
	{
		//1. 
		//Calculate the passedTicks (see the lab document)
		auto passedTicks = sceneContext.pGameTime->GetElapsed() * m_CurrentClip.ticksPerSecond * m_AnimationSpeed;

		//Make sure that the passedTicks stay between the m_CurrentClip.Duration bounds (fmod)
		if (passedTicks > m_CurrentClip.duration)
		{
			passedTicks = m_CurrentClip.duration;
		}

		//2.
		//IF m_Reversed is true
		if (m_Reversed)
		{
			//	Subtract passedTicks from m_TickCount
			m_TickCount -= passedTicks;
			//	If m_TickCount is smaller than zero, add m_CurrentClip.Duration to m_TickCount
			if (m_TickCount < 0)
			{
				m_TickCount += m_CurrentClip.duration;
			}
		}
		//ELSE
		else
		{
			//	Add passedTicks to m_TickCount
			m_TickCount += passedTicks;
			//	if m_TickCount is bigger than the clip duration, subtract the duration from m_TickCount
			if (m_TickCount > m_CurrentClip.duration)
			{
				m_TickCount -= m_CurrentClip.duration;
			}
		}

		//3.
		//Find the enclosing keys
		AnimationKey keyA{ m_CurrentClip.keys.front() };
		AnimationKey keyB{ m_CurrentClip.keys.front() };
		//Iterate all the keys of the clip and find the following keys:
		//keyA > Closest Key with Tick before/smaller than m_TickCount
		//keyB > Closest Key with Tick after/bigger than m_TickCount

		for (auto& key : m_CurrentClip.keys)
		{
			if (key.tick > m_TickCount)
			{
				keyB = key;
				break;
			}
			keyA = key;
		}

		//4.
		//Interpolate between keys
		//Figure out the BlendFactor (See lab document)
		auto blendFactor{ (m_TickCount - keyA.tick) / (keyB.tick - keyA.tick) };
		//Clear the m_Transforms vector
		m_Transforms.clear();

		//FOR every boneTransform in a key (So for every bone)
		auto boneSize{ int(keyA.boneTransforms.size()) };
		for (int bone{ 0 }; bone < boneSize; ++bone)
		{
			//	Retrieve the transform from keyA (transformA)
			auto transformA = keyA.boneTransforms[bone];
			// 	Retrieve the transform from keyB (transformB)
			auto transformB = keyB.boneTransforms[bone];
			//	Decompose both transforms
			auto matrixA{ XMLoadFloat4x4(&transformA) };
			XMVECTOR vecScaleA, vecRotA, vecTransA;
			XMMatrixDecompose(&vecScaleA, &vecRotA, &vecTransA, matrixA);
			auto matrixB{ XMLoadFloat4x4(&transformB) };
			XMVECTOR vecScaleB, vecRotB, vecTransB;
			XMMatrixDecompose(&vecScaleB, &vecRotB, &vecTransB, matrixB);
			//	Lerp between all the transformations (Position, Scale, Rotation)
			auto vecScaleAB{ XMVectorLerp(vecScaleA, vecScaleB, blendFactor) };
			auto vecTransAB{ XMVectorLerp(vecTransA, vecTransB, blendFactor) };
			auto vecRotAB{ XMQuaternionSlerp(vecRotA, vecRotB, blendFactor) };
			//	Compose a transformation matrix with the lerp-results
			auto resultMatrix{ XMMatrixTransformation(
				FXMVECTOR{},
				FXMVECTOR{},
				vecScaleAB,
				FXMVECTOR{},
				vecRotAB,
				vecTransAB
			) };
			//	Add the resulting matrix to the m_Transforms vector
			XMFLOAT4X4 resultFloat4x4{};
			XMStoreFloat4x4(&resultFloat4x4, resultMatrix);
			m_Transforms.push_back(resultFloat4x4);
		}
	}
}

void ModelAnimator::SetAnimation(const std::wstring& clipName)
{
	//Set m_ClipSet to false
	m_ClipSet = false;
	//Iterate the m_AnimationClips vector and search for an AnimationClip with the given name (clipName)
	for (auto& animationClips : m_pMeshFilter->m_AnimationClips)
	{
		//If found,
		if (animationClips.name == clipName)
		{
			//	Call SetAnimation(Animation Clip) with the found clip
			SetAnimation(animationClips);
			return;
		}
	}
	//Else
	//	Call Reset
	Reset(true);
	//	Log a warning with an appropriate message
	Logger::LogWarning(L"Clipname {} not found", clipName);
}

void ModelAnimator::SetAnimation(UINT clipNumber)
{
	//Set m_ClipSet to false
	m_ClipSet = false;
	//Check if clipNumber is smaller than the actual m_AnimationClips vector size
	if (clipNumber < m_pMeshFilter->m_AnimationClips.size())
	{
		//	Retrieve the AnimationClip from the m_AnimationClips vector based on the given clipNumber
		//	Call SetAnimation(AnimationClip clip)
		SetAnimation(m_pMeshFilter->m_AnimationClips[clipNumber]);
	}
	else
		//If not,
	{
		//	Call Reset
		Reset(true);
		//	Log a warning with an appropriate message
		Logger::LogWarning(L"ClipNumber is bigger than the actual m_AnimationClips vector size");
	}
	//	return
}

void ModelAnimator::SetAnimation(const AnimationClip& clip)
{
	//Set m_ClipSet to true
	m_ClipSet = true;
	//Set m_CurrentClip
	m_CurrentClip = clip;
	//Call Reset(false)
	Reset(false);

}

void ModelAnimator::Reset(bool pause)
{
	//If pause is true, set m_IsPlaying to false
	if (pause) m_IsPlaying = false;
	//Set m_TickCount to zero
	m_TickCount = 0;
	//Set m_AnimationSpeed to 1.0f
	m_AnimationSpeed = 1.0f;

	//If m_ClipSet is true
	if (m_ClipSet)
	{
		//	Retrieve the BoneTransform from the first Key from the current clip (m_CurrentClip)
		//	Refill the m_Transforms vector with the new BoneTransforms (have a look at vector::assign)
		m_Transforms.assign(m_CurrentClip.keys[0].boneTransforms.begin(), m_CurrentClip.keys[0].boneTransforms.end());
	}
	else
		//Else
	{
		//	Create an IdentityMatrix 
		//	Refill the m_Transforms vector with this IdenityMatrix (Amount = BoneCount) (have a look at vector::assign)
		XMFLOAT4X4 identity{
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f
		};
		m_Transforms.assign(m_pMeshFilter->m_BoneCount, identity);
	}
}
