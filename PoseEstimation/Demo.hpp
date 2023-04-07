#pragma once
#include "Runnable.hpp"
#include "World.hpp"

class Demo final : public Runnable
{
	// Variables
	World DemoWorld{};

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
