#pragma once
class SpikeMaterial final : public Material<SpikeMaterial>
{
public:
	SpikeMaterial();
	~SpikeMaterial() override = default;

	SpikeMaterial(const SpikeMaterial& other) = delete;
	SpikeMaterial(SpikeMaterial&& other) noexcept = delete;
	SpikeMaterial& operator=(const SpikeMaterial& other) = delete;
	SpikeMaterial& operator=(SpikeMaterial&& other) noexcept = delete;

	void SetDiffuseTexture(const std::wstring& assetFile);

protected:
	void InitializeEffectVariables() override;
};