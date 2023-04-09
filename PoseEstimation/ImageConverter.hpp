#pragma once

class ImageConverter
{
	// Variables
	GLuint Texture;
	int Width;
	int Height;

public:
	// Methods
	ImageConverter();
	void LoadMat(const void* frameId, int width, int height);
	void LoadMat(const cv::Mat& frame);
	void UpdateMat(const void* frameId, int width, int height);
	void UpdateMat(const cv::Mat& frame);
	void* GetTexture() const;
	void Clear();
};
