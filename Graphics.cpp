#include "Graphics.h" 
#include <d3dcompiler.h>
#include "dxerr.h"
#include <sstream>
#include <DirectXMath.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")



namespace wrl=Microsoft::WRL;
namespace dx = DirectX;

Graphics::Graphics(HWND hwnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;
	sd.OutputWindow = hwnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = 0;

	UINT swapCreateFlags = 0u;
#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	

	HRESULT hr;

	GFX_THROW_FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, swapCreateFlags, NULL, 0, D3D11_SDK_VERSION, &sd, &pSwap, &pDevice, NULL, &pContext));

	wrl::ComPtr<ID3D11Resource> pBackBuffer;
	GFX_THROW_INFO(pSwap->GetBuffer(0, _uuidof(ID3D11Resource), &pBackBuffer));
	GFX_THROW_INFO(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget));
	D3D11_VIEWPORT vp;
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1u, &vp);
	//nowe
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	wrl::ComPtr<ID3D11DepthStencilState> pDSState;
	GFX_THROW_INFO(pDevice->CreateDepthStencilState(&dsDesc, &pDSState));
	pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = 800u;
	descDepth.Height = 600u;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	GFX_THROW_INFO(pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	GFX_THROW_INFO(pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &pDSV));
}

void Graphics::EndFrame()
{
	HRESULT hr;
#ifndef NDEBUG
	infoManager.Set();
#endif
	if (FAILED(hr = pSwap->Present(1u, 0u)))
	{
		if (hr==DXGI_ERROR_DEVICE_REMOVED)
		{
				throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
		}
		else
		{
			throw GFX_EXCEPT(hr);
		}
	}
}
/*
void Graphics::DrawTriangle(float alfa, float x, float y)
{
	namespace wrl = Microsoft::WRL;
	HRESULT hr;
	struct Wierzcholek
	{
		struct
		{ 
			float x;
			float y;
			float z;
		} pos;
		struct 
		{ 
			unsigned char r; 
			unsigned char g;
			unsigned char b;
			unsigned char a;
		} color;
	};
	Wierzcholek w[] =
	{
		/*
		{0, 0.5, 255, 0, 0, 1},
		{0.5, -0.5, 0, 255, 0, 1},
		{-0.5, -0.5, 0, 0, 255, 1},
		{-0.3,	0.3, 0, 255, 0, 1},
		{0.3, 0.3, 0, 0, 255, 1},
		{0, -1, 255, 0, 0, 1}
		

		
		{-1, -1, -1,	 255, 0, 0, 0},
		{1, -1, -1,		 0, 255, 0, 0},
		{-1, 1, -1,		 0, 0, 255, 0},
		{1, 1, -1,		 255, 255, 0, 0},
		{-1, -1, 1,		 255, 0, 255, 0},
		{1, -1, 1,		 0, 255, 255, 0},
		{-1, 1, 1,		 0, 0, 0, 0},
		{1, 1, 1,		 255, 255, 255, 0}
		
	};
	w[0].color.g = 255;
	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(w);
	bd.StructureByteStride = sizeof(Wierzcholek);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = w;
	GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));

	UINT stride = sizeof(Wierzcholek);
	UINT offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
	
	//ibuffer
	const unsigned short ind[] =
	{
		/*
		0, 1, 2,
		0, 2, 3,
		0, 4, 1,
		2, 1, 5
		
		
		0, 2, 1,	2, 3, 1,
		1, 3, 5,	3, 7, 5,
		2, 6, 3,	3, 6, 7,	
		4, 5, 7,	4, 7, 6,
		0, 4, 2,	2, 4, 6,
		0, 1, 4,	1, 5, 4
		
		
	};
	wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
	D3D11_BUFFER_DESC ibd = {};
	D3D11_SUBRESOURCE_DATA isd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(ind);
	ibd.StructureByteStride = sizeof(unsigned short);
	isd.pSysMem = ind;
	GFX_THROW_INFO(pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer));

	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	//macierz transformacji
	struct ConstBuffer
	{
		dx::XMMATRIX transform;
	};	

	const ConstBuffer cb
	{
		{
			//*dx::XMMatrixTranslation(0, 0, -4) *dx::XMMatrixPerspectiveLH(1, 0.75, 0.5, 10)	
			dx::XMMatrixTranspose
			(
				dx::XMMatrixRotationZ(alfa) * 
				//dx::XMMatrixScaling(3/4.0, 1, 0) *
				dx::XMMatrixRotationX(alfa) * 
				dx::XMMatrixPerspectiveLH(1, 0.75, 0.5, 10) *
				dx::XMMatrixTranslation(x, y, 4)
			)
		}
	};

	wrl::ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC cbd = {};
	D3D11_SUBRESOURCE_DATA csd = {};
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cb);
	cbd.StructureByteStride = 0u;
	csd.pSysMem = &cb;	
	GFX_THROW_INFO(pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer));

	pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());

	//kolor
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	wrl::ComPtr<ID3DBlob> pBlob;
	GFX_THROW_INFO(D3DReadFileToBlob(L"PixelShader.cso", &pBlob));
	GFX_THROW_INFO(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0);

	//shader
	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	GFX_THROW_INFO(D3DReadFileToBlob(L"VertexShader.cso", &pBlob));
	GFX_THROW_INFO(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));
	pContext->VSSetShader(pVertexShader.Get(),nullptr ,0);


	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);
	//inpl
	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] = 
	{ 
		{"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}, 
		{"Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	GFX_THROW_INFO(pDevice->CreateInputLayout(ied, (UINT)std::size(ied), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout));
	pContext->IASetInputLayout(pInputLayout.Get());
	//zamiana trojkata na obiekt d3d
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//viewport setup
	D3D11_VIEWPORT vp;
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1u, &vp);

	GFX_THROW_INFO_ONLY(pContext->DrawIndexed((UINT)std::size(ind), 0u, 0u));
}*/

void Graphics::ClearBuffer(float r, float g, float b) noexcept
{
	const float color[] = { r,g,b,1 };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
	pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1u, 0u);
}

Graphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string>infoMsgs) noexcept : Exception(line, file), hr(hr)  
{
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}
	if (!info.empty())
	{
		info.pop_back();
	}
}

const char* Graphics::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << "\nError Code : 0x" << std::hex << std::uppercase << GetErrorCode() << std::dec << " (" << (ULONG)GetErrorCode() << ")\nError String : " << GetErrorString() << "\nDescription : " << GetErrorDescription() << "\n";
	if (!info.empty())
	{
		oss << "\nError Info: \n" << GetErrorInfo() << "\n\n";
	}
	oss<< GetOriginString();
	std::string whatBuffer = oss.str();
	return whatBuffer.c_str();
}


HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string Graphics::HrException::GetErrorString() const noexcept
{
	std::wstring ws = DXGetErrorString(hr);
	std::string str(ws.begin(), ws.end());
	return str;
}

std::string Graphics::HrException::GetErrorDescription() const noexcept
{
	WCHAR buf[512];
	DXGetErrorDescription(hr, buf, sizeof(buf));
	std::wstring ws = std::wstring(buf);
	std::string str(ws.begin(), ws.end());
	return str;
}

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "Device Removed : (DXGI_ERROR_DEVICE_REMOVED)";
}

const char* Graphics::HrException::GetType() const noexcept
{
	return "Graphical Hr Window Exception";
}

std::string Graphics::HrException::GetErrorInfo() const noexcept
{
	return info;
}

const char* Graphics::InfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	oss << GetOriginString();
	std::string whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::InfoException::GetType() const noexcept
{
	return "Chili Graphics Info Exception";
}

std::string Graphics::InfoException::GetErrorInfo() const noexcept
{
	return info;
}

Graphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
	:
	Exception(line, file)
{
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}
	// remove final newline if exists
	if (!info.empty())
	{
		info.pop_back();
	}
}

void Graphics::Start()
{
	start = std::clock();
}

void Graphics::Stop()
{
	stop = std::clock();
}

float Graphics::Elapsed()
{
	return (stop - start) / (float)CLOCKS_PER_SEC;
}
void::Graphics::DrawIndexed(UINT count) noexcept
{
	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());
	GFX_THROW_INFO_ONLY(pContext->DrawIndexed(count, 0u, 0u));
}
void Graphics::SetProjection(DirectX::FXMMATRIX proj) noexcept
{
	projection = proj;
}

DirectX::XMMATRIX Graphics::GetProjection() const noexcept
{
	return projection;
}