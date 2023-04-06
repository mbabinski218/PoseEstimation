#pragma once
#include "Runnable.hpp"
#include "World.hpp"

class Demo final : public Runnable
{
	// Variables
	World DemoWorld{};
	float DeltaTime = 0.0f;
	float LastFrame = 0.0f;

	inline static bool Show3dModel = true;
	inline static bool Show3dModelControls = true;

	// Methods
public:
	explicit Demo();
	~Demo() override;

private:
	void Loop() override;
	void HandleInput() override;
};
