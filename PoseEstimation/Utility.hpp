#pragma once
#include "Libs.hpp"

enum DnnTargetMode
{
	CPU,
	CUDA
};

template<typename T>
struct CameraLinker
{
	T Link;
	explicit CameraLinker(T link) : Link(link) { }
};