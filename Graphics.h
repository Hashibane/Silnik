#pragma once
#include "NHWindows.h"
#include <wrl.h>
#include "EditedExceptions.h"
#include <d3d11.h>
#include "DxgiInfoManager.h"
#include <cstdio>
#include <ctime>
#include "GMacros.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
class Graphics
{
	friend class Bindable;
	public:
		class exce : public Exception
		{
			using Exception::Exception;
		};
		class HrException : public Exception
		{
			public:
				HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
				const char* what() const noexcept override;
				const char* GetType() const noexcept override;
				HRESULT GetErrorCode() const noexcept;
				std::string GetErrorString() const noexcept;
				std::string GetErrorDescription() const noexcept;
				std::string GetErrorInfo() const noexcept;
			private:
				HRESULT hr;
				std::string info;
		};
		class DeviceRemovedException : public HrException
		{
				using HrException::HrException;
			public:
				const char* GetType() const noexcept override;
			private:
				std::string reason;
		};
		class InfoException : public Exception
		{
		public:
			InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;
			const char* what() const noexcept override;
			const char* GetType() const noexcept override;
			std::string GetErrorInfo() const noexcept;
		private:
			std::string info;
		};
		Graphics(HWND hwnd);
		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;
		void EndFrame();
		void ClearBuffer(float r, float g, float b) noexcept;
		void DrawTriangle(float alfa, float x, float y);
		std::clock_t start;
		std::clock_t stop;
		void Start();
		void Stop();
		float Elapsed();
	
		void DrawIndexed(UINT count) noexcept;
		void SetProjection(DirectX::FXMMATRIX proj) noexcept;
		DirectX::XMMATRIX GetProjection() const noexcept;
	private:
		DirectX::XMMATRIX projection;
		#ifndef NDEBUG
			DxgiInfoManager infoManager;
		#endif
			Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
			Microsoft::WRL::ComPtr < IDXGISwapChain> pSwap;
			Microsoft::WRL::ComPtr < ID3D11DeviceContext> pContext;
			Microsoft::WRL::ComPtr < ID3D11RenderTargetView> pTarget;
			Microsoft::WRL::ComPtr <ID3D11DepthStencilView> pDSV;
};

