#pragma once

#ifdef _WIN32
#include "Windows.h"
#endif

#include "glew/glew.h"
#include "GLFW/glfw3.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include "imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/quaternion.hpp"
#include <exception>
#include <fstream>
#include <string>
#include <boost/exception/diagnostic_information.hpp> 
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>