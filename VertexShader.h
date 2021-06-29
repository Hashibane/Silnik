#pragma once
#include "Bindable.h"

class VertexShader : public Bindable
{
public:
	VertexShader(Graphics& gfx, const std::string& path);
	void Bind(Graphics& gfx) noexcept override;
	ID3DBlob* GetBytecode() const noexcept
	{
		return pBytecodeBlob.Get();
	}
protected:
	std::string path;
	Microsoft::WRL::ComPtr<ID3DBlob> pBytecodeBlob;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
};