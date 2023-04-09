#pragma once

#ifdef _WIN32
#include <Windows.h>
#endif

#include <glew/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/hash.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/quaternion.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <boost/exception/diagnostic_information.hpp> 

#include <unordered_map>
#include <filesystem>
#include <exception>
#include <iostream>
#include <fstream>
#include <string>
#include <ranges>
#include <memory>
#include <vector>
#include <map>