#include "PoseEstimation.hpp"

// Methods
std::shared_ptr<cv::dnn::Net> PoseEstimation::CreateDnnNet(const std::string& protoTextPath, const std::string& caffeModel, const DnnTargetMode& dnnMode)
{
    const auto currentPath = std::filesystem::current_path().string();
    auto net = cv::dnn::readNetFromCaffe(currentPath + protoTextPath, currentPath + caffeModel);

    switch (dnnMode)
    {
    case CPU:
        net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
        break;
    case CUDA:
        net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
        net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
        break;
    default:
        throw std::exception("Dnn target mode not set");
    }

    if (net.empty())
        throw std::exception("Error when reading model");

    return std::make_shared<cv::dnn::Net>(net);
}

PoseEstimation::PoseEstimation(std::shared_ptr<cv::dnn::Net> net, const ImVec2& size, const int& poseParts,std::vector<std::vector<int>> posePairs, const double& threshHold) :
	Net(std::move(net)),
	Image(std::make_unique<ImageConverter>()),
	OutputBlob(std::make_unique<cv::Mat>()),
	PosePairs(std::move(posePairs)),
	ThreshHold(threshHold), PoseParts(poseParts)
{
    CameraSize = cv::Size2f(size.x, size.y);
}

void PoseEstimation::FindPose(const std::unique_ptr<cv::Mat>& mat) const
{
	const auto inputBlob = cv::dnn::blobFromImage(*mat, 1.0 / 255.0, cv::Size(299, 299), cv::Scalar(0, 0, 0));
	Net->setInput(inputBlob);
    *OutputBlob = Net->forward();
}

void PoseEstimation::Update(const std::unique_ptr<cv::Mat>& mat) const
{
	const auto thread = std::make_unique<std::jthread>([&]() 
	{
		FindPose(mat);
        AddPoseToImage(mat);
	});
	thread->join();

	Image->UpdateMat(*mat);
}

void* PoseEstimation::GetTexture() const
{
	return Image->GetTexture();
}

void PoseEstimation::AddPoseToImage(const std::unique_ptr<cv::Mat>& mat) const
{
    const auto height = OutputBlob->size[2];
    const auto width = OutputBlob->size[3];
    std::vector<cv::Point> points(PoseParts);

    for (int n = 0; n < PoseParts; n++)
    {
        cv::Mat probMap(height, width, CV_32F, OutputBlob->ptr(0, n));
        cv::Point2f p(-1, -1);
        cv::Point maxLoc;
        double maxVal;

        minMaxLoc(probMap, nullptr, &maxVal, nullptr, &maxLoc);

        if (maxVal > ThreshHold)
        {
            p = maxLoc;
            p.x *= CameraSize.width / static_cast<float>(width);
            p.y *= CameraSize.height / static_cast<float>(height);
        }
        points[n] = p;
    }

    for (const auto& posePair : PosePairs)
    {
        cv::Point2f partA = points[posePair[0]];
        cv::Point2f partB = points[posePair[1]];

        if (partA.x <= 0 || partA.y <= 0 || partB.x <= 0 || partB.y <= 0)
            continue;

        line(*mat, partA, partB, cv::Scalar(65, 115, 190), 5);
        circle(*mat, partA, 4, cv::Scalar(220, 220, 220), 1);
        circle(*mat, partB, 4, cv::Scalar(220, 220, 220), 1);
        putText(*mat, std::to_string(posePair[0]), partA, 0, 0.5, cv::Scalar(0, 0, 0), 2);
        putText(*mat, std::to_string(posePair[1]), partB, 0, 0.5, cv::Scalar(0, 0, 0), 2);
    }
}