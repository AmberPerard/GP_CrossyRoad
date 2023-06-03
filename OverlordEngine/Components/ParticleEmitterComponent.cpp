#include "stdafx.h"
#include "ParticleEmitterComponent.h"
#include "Misc/ParticleMaterial.h"

ParticleMaterial* ParticleEmitterComponent::m_pParticleMaterial{};

ParticleEmitterComponent::ParticleEmitterComponent(const std::wstring& assetFile, const ParticleEmitterSettings& emitterSettings, UINT particleCount, bool singlePlay) :
	m_ParticlesArray(new Particle[particleCount]),
	m_ParticleCount(particleCount), //How big is our particle buffer?
	m_MaxParticles(particleCount), //How many particles to draw (max == particleCount)
	m_AssetFile(assetFile),
	m_EmitterSettings(emitterSettings),
	m_IsSinglePlay(singlePlay)
{
	m_enablePostDraw = true; //This enables the PostDraw function for the component
}

ParticleEmitterComponent::~ParticleEmitterComponent()
{
	//TODO_W9(L"Implement Destructor")
	SafeDelete(m_ParticlesArray);
	SafeRelease(m_pVertexBuffer);
}

void ParticleEmitterComponent::Initialize(const SceneContext& sceneContext)
{
	//TODO_W9(L"Implement Initialize")
	if (m_pParticleMaterial == nullptr)
	{
		m_pParticleMaterial = MaterialManager::Get()->CreateMaterial<ParticleMaterial>();
	}

	CreateVertexBuffer(sceneContext);
	m_pParticleTexture = ContentManager::Load<TextureData>(m_AssetFile);
}

void ParticleEmitterComponent::CreateVertexBuffer(const SceneContext& sceneContext)
{
	//TODO_W9(L"Implement CreateVertexBuffer")
	if (m_pVertexBuffer)
	{
		SafeRelease(m_pVertexBuffer);
	}

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	bd.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.ByteWidth = sizeof(VertexParticle) * m_ParticleCount;
	bd.StructureByteStride = sizeof(VertexParticle);
	bd.MiscFlags = 0;

	HRESULT hr{ sceneContext.d3dContext.pDevice->CreateBuffer(&bd, nullptr, &m_pVertexBuffer) };
	if (FAILED(hr))
	{
		HANDLE_ERROR(L"Failed to create vertexbuffer, ParticleEmitterComponent");
		return;
	}
}

void ParticleEmitterComponent::Update(const SceneContext& sceneContext)
{
	const float elapsedTime = sceneContext.pGameTime->GetElapsed();
	//TODO_W9(L"Implement Update")

	float particleInterval{ (m_EmitterSettings.maxEnergy + m_EmitterSettings.minEnergy) / m_ParticleCount };


	m_LastParticleSpawn += elapsedTime;

	m_ActiveParticles = 0;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	sceneContext.d3dContext.pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);


	VertexParticle* pBuffer = reinterpret_cast<VertexParticle*>(mappedResource.pData);

	for (size_t i = 0; i < m_ParticleCount; i++)
	{
		if (m_ParticlesArray[i].isActive)
		{
			UpdateParticle(m_ParticlesArray[i], elapsedTime);
		}

		if (!m_IsSinglePlay)
		{
			//prevents particle spawning when singleplay is true
			if (!m_ParticlesArray[i].isActive && m_LastParticleSpawn >= particleInterval)
			{
				SpawnParticle(m_ParticlesArray[i]);
			}
		}

		if (m_ParticlesArray[i].isActive)
		{
			pBuffer[m_ActiveParticles] = m_ParticlesArray[i].vertexInfo;
			m_ActiveParticles++;
		}
	}

	sceneContext.d3dContext.pDeviceContext->Unmap(m_pVertexBuffer, 0);
}

void ParticleEmitterComponent::UpdateParticle(Particle& p, float elapsedTime) const
{
	//TODO_W9(L"Implement UpdateParticle")

	if (!p.isActive)return;

	p.currentEnergy -= elapsedTime;
	if (p.currentEnergy < 0)
	{
		p.isActive = false;
		return;
	}

	XMStoreFloat3(&p.vertexInfo.Position, XMLoadFloat3(&p.vertexInfo.Position) + (XMLoadFloat3(&m_EmitterSettings.velocity) * elapsedTime));

	float lifePercent = p.currentEnergy / p.totalEnergy;

	p.vertexInfo.Color = m_EmitterSettings.color;
	p.vertexInfo.Color.w = lifePercent * 2.0f;

	if (p.sizeChange < 1.0)
	{
		p.vertexInfo.Size = p.initialSize + p.sizeChange * (1 - lifePercent);
	}
	else
	{
		p.vertexInfo.Size = p.initialSize + p.sizeChange * lifePercent;
	}
}

void ParticleEmitterComponent::SpawnParticle(Particle& p)
{
	//TODO_W9(L"Implement SpawnParticle")
	p.isActive = true;

	p.totalEnergy = MathHelper::randF(m_EmitterSettings.minEnergy, m_EmitterSettings.maxEnergy);
	p.currentEnergy = p.totalEnergy;

	XMVECTOR randomDir{ 1,0,0 };

	randomDir = XMVector3TransformNormal(randomDir, XMMatrixRotationRollPitchYaw(MathHelper::randF(-XM_PI, XM_PI), MathHelper::randF(-XM_PI, XM_PI), MathHelper::randF(-XM_PI, XM_PI)));

	float randomDistance{ MathHelper::randF(m_EmitterSettings.minEmitterRadius, m_EmitterSettings.maxEmitterRadius) };

	XMStoreFloat3(&p.vertexInfo.Position, XMLoadFloat3(&GetTransform()->GetWorldPosition()) + randomDir * randomDistance);

	p.vertexInfo.Size = MathHelper::randF(m_EmitterSettings.minSize, m_EmitterSettings.maxSize);
	p.initialSize = p.vertexInfo.Size;

	p.sizeChange = MathHelper::randF(m_EmitterSettings.minScale, m_EmitterSettings.maxScale);

	p.vertexInfo.Rotation = MathHelper::randF(-XM_PI, XM_PI);

	p.vertexInfo.Color = m_EmitterSettings.color;
}

void ParticleEmitterComponent::PostDraw(const SceneContext& sceneContext)
{
	//TODO_W9(L"Implement PostDraw")
	m_pParticleMaterial->SetVariable_Matrix(L"gWorldViewProj", sceneContext.pCamera->GetViewProjection());

	m_pParticleMaterial->SetVariable_Matrix(L"gViewInverse", sceneContext.pCamera->GetViewInverse());

	m_pParticleMaterial->SetVariable_Texture(L"gParticleTexture", m_pParticleTexture);

	MaterialTechniqueContext tech = m_pParticleMaterial->GetTechniqueContext();

	sceneContext.d3dContext.pDeviceContext->IASetInputLayout(tech.pInputLayout);

	sceneContext.d3dContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	UINT stride{ sizeof(VertexParticle) };
	UINT offset{ 0 };
	sceneContext.d3dContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	D3DX11_TECHNIQUE_DESC techDesc;
	tech.pTechnique->GetDesc(&techDesc);
	for (UINT i{ 0 }, passes{ techDesc.Passes }; i < passes; ++i)
	{

		tech.pTechnique->GetPassByIndex(i)->Apply(0, sceneContext.d3dContext.pDeviceContext);
		sceneContext.d3dContext.pDeviceContext->Draw(m_ActiveParticles, 0);
	}
}

void ParticleEmitterComponent::DrawImGui()
{
	if (ImGui::CollapsingHeader("Particle System"))
	{
		ImGui::SliderUInt("Count", &m_ParticleCount, 0, m_MaxParticles);
		ImGui::InputFloatRange("Energy Bounds", &m_EmitterSettings.minEnergy, &m_EmitterSettings.maxEnergy);
		ImGui::InputFloatRange("Size Bounds", &m_EmitterSettings.minSize, &m_EmitterSettings.maxSize);
		ImGui::InputFloatRange("Scale Bounds", &m_EmitterSettings.minScale, &m_EmitterSettings.maxScale);
		ImGui::InputFloatRange("Radius Bounds", &m_EmitterSettings.minEmitterRadius, &m_EmitterSettings.maxEmitterRadius);
		ImGui::InputFloat3("Velocity", &m_EmitterSettings.velocity.x);
		ImGui::ColorEdit4("Color", &m_EmitterSettings.color.x, ImGuiColorEditFlags_NoInputs);
	}
}

void ParticleEmitterComponent::SpawnOneShot()
{
	for (int i{}; i < (int)m_ParticleCount; ++i)
	{
		Particle& particle = m_ParticlesArray[i];
		SpawnParticle(particle);
	}
}
