#pragma once
#include "Drawable.h"
#include <random>
class Box : public Drawable
{
	public:
		//mt - mersenne twister engine (liczby losowe)
		Box
		(
			Graphics& gfx, 
			std::mt19937& rng, 
			std::uniform_real_distribution<float>& adist, 
			std::uniform_real_distribution<float>& ddist,
			std::uniform_real_distribution<float>& odist,
			std::uniform_real_distribution<float>& rdist
		);
		void Update(float dt) noexcept override;
		DirectX::XMMATRIX GetTransformXM() const noexcept override;
	private:
		//pozycja
		float r;
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float theta;
		float phi;
		float chi;
		//predkosci
		float droll;
		float dpitch;
		float dyaw;
		float dtheta;
		float dphi;
		float dchi;
};