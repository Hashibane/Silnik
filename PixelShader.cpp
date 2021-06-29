
#include "PixelShader.h"
#include "GMacros.h"
#include "BindableBase.h"


	PixelShader::PixelShader(Graphics& gfx, const std::string& path) : path(path)
	{
		INFOMAN(gfx);

		Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
		std::string str = path;
		std::wstring widestr = std::wstring(str.begin(), str.end());
		const wchar_t* widecstr = widestr.c_str();;
		GFX_THROW_INFO(D3DReadFileToBlob(widecstr, &pBlob));
		GFX_THROW_INFO(GetDevice(gfx)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
	}

	void PixelShader::Bind(Graphics& gfx) noexcept
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0u));
	}
