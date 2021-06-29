#pragma once
#include "Graphics.h"
#include <d3dcompiler.h>
#include <memory>
class Bindable
{
	public:
		virtual void Bind(Graphics& gfx) noexcept = 0;
		virtual ~Bindable() = default;
	protected:
		static ID3D11DeviceContext* GetContext(Graphics& gfx) noexcept
		{
			return gfx.pContext.Get();
		};
		static ID3D11Device* GetDevice(Graphics& gfx) noexcept
		{
			return gfx.pDevice.Get();
		};
		static DxgiInfoManager& GetInfoManager(Graphics& gfx) noexcept
		{
#ifndef NDEBUG
			return gfx.infoManager;
#else
			throw std::logic_error("logic error (tried to access gfx.infoManager in Release config)");
#endif
		};
};