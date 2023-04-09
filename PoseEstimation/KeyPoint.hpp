#pragma once

struct KeyPoint
{
	int Id;
	cv::Point Point;
	float Probability;

	KeyPoint(const cv::Point& point, const float& probability)
	{
		Id = -1;
		Point = point;
		Probability = probability;
	}
};
