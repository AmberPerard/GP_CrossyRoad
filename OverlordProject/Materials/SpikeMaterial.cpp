#include "stdafx.h"
#include "SpikeMaterial.h"

SpikeMaterial::SpikeMaterial()
	:Material<SpikeMaterial>(L"Effects/SpikyShader.fx")
{
}

void SpikeMaterial::SetDiffuseTexture(const std::wstring& /*assetFile*/)
{
}

void SpikeMaterial::InitializeEffectVariables()
{
}
