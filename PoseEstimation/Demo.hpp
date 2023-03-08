#pragma once
#include "Runnable.hpp"
#include "Mesh.hpp"

class Demo : public Runnable
{
	// Variables
	std::unique_ptr<Mesh> Model;

	ImVec2 Padding;

	inline static bool Show3dModel = true;
	inline static bool Show3dModelControls = true;

	// Methods
public:
	explicit Demo();
	~Demo() override;

private:
	void Loop() const override;
	void HandleInput() const override;
};
