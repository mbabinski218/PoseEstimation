#include "PoseEstimation.hpp"

constexpr int POSE_PAIRS[17][2] =
{
    {1,2}, {1,5}, {2,3},
    {3,4}, {5,6}, {6,7},
    {1,8}, {8,9}, {9,10},
    {1,11}, {11,12}, {12,13},
    {1,0}, {0,14},
    {14,16}, {0,15}, {15,17}
};

// Methods
PoseEstimation::PoseEstimation(const std::string& protoTextPath, const std::string& caffeModel, const DnnTargetMode& dnnMode)
	: Image(std::make_unique<ImageConverter>())
{
	const auto currentPath = std::filesystem::current_path().string();
	auto net = cv::dnn::readNetFromCaffe(currentPath + protoTextPath, currentPath + caffeModel);

    switch(dnnMode)
    {
    case Cpu: 
        net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
		break;
    case Cuda:
        net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
        net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
        break;
    default:
        throw std::exception("Dnn target mode error");
    }
	
	Net = std::make_unique<cv::dnn::Net>(net);

	if (net.empty())
		throw std::exception("Error when reading model");
}

//void GetKeyPoints(cv::Mat& probMap, const double threshold, std::vector<KeyPoint>& keyPoints)
//{
//	cv::Mat smoothProbMap;
//	cv::GaussianBlur(probMap, smoothProbMap, cv::Size(3, 3), 0, 0);
//
//	cv::Mat maskedProbMap;
//	cv::threshold(smoothProbMap, maskedProbMap, threshold, 255, cv::THRESH_BINARY);
//
//	maskedProbMap.convertTo(maskedProbMap, CV_8U, 1);
//
//	std::vector<std::vector<cv::Point> > contours;
//	cv::findContours(maskedProbMap, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
//
//	for (auto& contour : contours)
//	{
//		cv::Mat blobMask = cv::Mat::zeros(smoothProbMap.rows, smoothProbMap.cols, smoothProbMap.type());
//
//		cv::fillConvexPoly(blobMask, contour, cv::Scalar(1));
//
//		double maxVal;
//		cv::Point maxLoc;
//
//		cv::minMaxLoc(smoothProbMap.mul(blobMask), 0, &maxVal, 0, &maxLoc);
//
//		keyPoints.emplace_back(maxLoc, probMap.at<float>(maxLoc.y, maxLoc.x));
//	}
//}

//void PoseEstimation::SplitNetOutputBlobToParts(cv::Mat& netOutputBlob, const cv::Size& targetSize, std::vector<cv::Mat>& netOutputParts) const
//{
//	const int nParts = netOutputBlob.size[1];
//	const int h = netOutputBlob.size[2];
//	const int w = netOutputBlob.size[3];
//
//	for (int i = 0; i < nParts; ++i) {
//		cv::Mat part(h, w, CV_32F, netOutputBlob.ptr(0, i));
//
//		cv::Mat resizedPart;
//
//		cv::resize(part, resizedPart, targetSize);
//
//		netOutputParts.push_back(resizedPart);
//	}
//}

std::unique_ptr<cv::Mat> PoseEstimation::FindPose(const std::unique_ptr<cv::Mat>& mat) const
{
	std::unique_ptr<cv::Mat> outputBlob;

	const auto thread = std::make_unique<std::jthread>([&]() {
		const auto inputBlob = cv::dnn::blobFromImage(*mat, 1.0 / 255.0, cv::Size(640, 480), cv::Scalar(0, 0, 0));
		Net->setInput(inputBlob);
		outputBlob = std::make_unique<cv::Mat>(Net->forward());
	});
	thread->join();

	return outputBlob;
}

void PoseEstimation::Create(const std::unique_ptr<cv::Mat>& mat) const
{	
	Image->LoadCvMat(*mat);
}

void PoseEstimation::Update(const std::unique_ptr<cv::Mat>& mat) const
{
	const auto thread = std::make_unique<std::jthread>([&]() 
	{
		const auto outputBlob = FindPose(mat);
		ConvertPoseToImage(outputBlob, mat);
	});
	thread->join();

	Image->UpdateMat(*mat);
}

void* PoseEstimation::GetTexture() const
{
	return Image->GetTexture();
}

void PoseEstimation::ConvertPoseToImage(const std::unique_ptr<cv::Mat>& outputBlob, const std::unique_ptr<cv::Mat>& mat) const
{
    const int H = outputBlob->size[2];
    const int W = outputBlob->size[3];

    // find the position of the body parts
    std::vector<cv::Point> points(18);
    for (int n = 0; n < 18; n++)
    {
        // Probability map of corresponding body's part.
        cv::Mat probMap(H, W, CV_32F, outputBlob->ptr(0, n));

        cv::Point2f p(-1, -1);
        cv::Point maxLoc;
        double prob;
        minMaxLoc(probMap, 0, &prob, 0, &maxLoc);
        if (prob > 0.1)
        {
            p = maxLoc;
            p.x *= (float)640 / W;
            p.y *= (float)480 / H;
        }
        points[n] = p;
    }

    int nPairs = sizeof(POSE_PAIRS) / sizeof(POSE_PAIRS[0]);

    for (int n = 0; n < nPairs; n++)
    {
        // lookup 2 connected body/hand parts
        cv::Point2f partA = points[POSE_PAIRS[n][0]];
        cv::Point2f partB = points[POSE_PAIRS[n][1]];

        if (partA.x <= 0 || partA.y <= 0 || partB.x <= 0 || partB.y <= 0)
            continue;

        line(*mat, partA, partB, cv::Scalar(0, 255, 255), 8);
        circle(*mat, partA, 8, cv::Scalar(0, 0, 255), -1);
        circle(*mat, partB, 8, cv::Scalar(0, 0, 255), -1);
    }
}

//void PoseEstimation::ConvertPoseToImage(const std::unique_ptr<cv::Mat>& outputBlob, const std::unique_ptr<cv::Mat>& mat) const
//{
//	std::vector<cv::Mat> netOutputParts;
//	SplitNetOutputBlobToParts(*outputBlob, cv::Size(640, 480), netOutputParts);
//
//	int keyPointId = 0;
//	std::vector<std::vector<KeyPoint>> detectedKeypoints;
//	std::vector<KeyPoint> keyPointsList;
//
//	for (int i = 0; i < 18; ++i) {
//		std::vector<KeyPoint> keyPoints;
//
//		GetKeyPoints(netOutputParts[i], 0.1, keyPoints);
//
//		for (int i = 0; i < keyPoints.size(); ++i, ++keyPointId) {
//			keyPoints[i].Id = keyPointId;
//		}
//
//		detectedKeypoints.push_back(keyPoints);
//		keyPointsList.insert(keyPointsList.end(), keyPoints.begin(), keyPoints.end());
//	}
//
//	for (int i = 0; i < 18; ++i) {
//		for (const auto& j : detectedKeypoints[i])
//		{
//			cv::circle(*mat, j.Point, 5, cv::Scalar(255, 255, 255, 255), -1, cv::LINE_AA);
//		}
//	}
//}