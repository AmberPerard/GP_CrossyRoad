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

	void UseTransparency(bool enabled);
	void SetColor(const XMFLOAT4& color) const;

protected:
	void InitializeEffectVariables() override;
};