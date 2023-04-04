#include "stdafx.h"
#include "SpikeMaterial.h"

SpikeMaterial::SpikeMaterial()
	:Material<SpikeMaterial>(L"Effects/SpikyShader.fx")
{
}
void SpikeMaterial::UseTransparency(bool enabled)
{
	SetTechnique(enabled ? L"TransparencyTech" : L"Default");
}

void SpikeMaterial::SetColor(const XMFLOAT4& color) const
{
	SetVariable_Vector(L"gColor", color);
}

void SpikeMaterial::InitializeEffectVariables()
{
}
