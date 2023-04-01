#pragma once
#include "Runnable.hpp"
#include "Model.hpp"

class Demo final : public Runnable
{
	// Variables
	std::unique_ptr<std::vector<Model>> Models;

	Model DefaultModel;

	inline static bool Show3dModel = true;
	inline static bool Show3dModelControls = true;

	// Methods
public:
	explicit Demo();
	~Demo() override;

private:
	void Loop() override;
	void HandleInput() const override;
};
