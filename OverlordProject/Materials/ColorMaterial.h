#pragma once
class ColorMaterial final: public Material<ColorMaterial>
{
public:
	ColorMaterial();
	~ColorMaterial() override = default;

	ColorMaterial(const ColorMaterial& other) = delete;
	ColorMaterial(ColorMaterial&& other) noexcept = delete;
	ColorMaterial& operator=(const ColorMaterial& other) = delete;
	ColorMaterial& operator=(ColorMaterial&& other) noexcept = delete;

	void UseTransparency(bool enabled);
	void SetColor(const XMFLOAT4& color) const;
	void SetSpecularTexture(const std::wstring& assetFile);
	void SetNormalTexture(const std::wstring& assetFile);
	void SetCubeMap(const std::wstring& assetFile);
	void SetOpacityMap(const std::wstring& assetFile);

protected:
	void InitializeEffectVariables() override;

};

