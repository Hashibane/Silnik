#pragma once
#include "Graphics.h"
#include <DirectXMath.h>
#include <memory>

class Bindable;

class Drawable
{
	public:
		//ISDEBUG
		Drawable() = default;
		Drawable(const Drawable&) = delete;
		virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
		void Draw(Graphics& gfx) const noexcept;
		virtual void Update(float dt) noexcept = 0;
		void AddBind(std::unique_ptr<Bindable> ibuf) noexcept;
		void AddIndexBuffer(std::unique_ptr<class IndexBuffer> ibuf) noexcept;
		virtual ~Drawable() = default;
	private:
		const IndexBuffer* pIndexBuffer = nullptr;
		std::vector<std::unique_ptr<Bindable>> binds;
};