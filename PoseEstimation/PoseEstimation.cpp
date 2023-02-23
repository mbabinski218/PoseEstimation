#include "PoseEstimation.hpp"

// Methods
std::shared_ptr<cv::dnn::Net> PoseEstimation::CreateDnnNet(const std::string& protoTextPath, const std::string& caffeModel, const DnnTargetMode& dnnMode)
{
    const auto currentPath = std::filesystem::current_path().string();
    auto net = cv::dnn::readNetFromCaffe(currentPath + protoTextPath, currentPath + caffeModel);

    switch (dnnMode)
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

    if (net.empty())
        throw std::exception("Error when reading model");

    return std::make_unique<cv::dnn::Net>(net);
}

PoseEstimation::PoseEstimation(std::shared_ptr<cv::dnn::Net> net, const ImVec2& size, const int& poseParts, 
    const std::vector<std::vector<int>>& posePairs, const double& threshHold) : Net(std::move(net)),
	Image(std::make_unique<ImageConverter>()), PosePairs(posePairs), ThreshHold(threshHold), PoseParts(poseParts)
{
    Size = cv::Size2f(size.x, size.y);
}

std::unique_ptr<cv::Mat> PoseEstimation::FindPose(const std::unique_ptr<cv::Mat>& mat) const
{
	const auto inputBlob = cv::dnn::blobFromImage(*mat, 1.0 / 255.0, Size, cv::Scalar(0, 0, 0));
	Net->setInput(inputBlob);
    const auto outputBlob = Net->forward();

	return std::make_unique<cv::Mat>(outputBlob);
}

void PoseEstimation::Update(const std::unique_ptr<cv::Mat>& mat) const
{
	const auto thread = std::make_unique<std::jthread>([&]() 
	{
		const auto outputBlob = FindPose(mat);
        AddPoseToImage(outputBlob, mat);
	});
	thread->join();

	Image->UpdateMat(*mat);
}

void* PoseEstimation::GetTexture() const
{
	return Image->GetTexture();
}

void PoseEstimation::AddPoseToImage(const std::unique_ptr<cv::Mat>& outputBlob, const std::unique_ptr<cv::Mat>& mat) const
{
    const auto h = outputBlob->size[2];
    const auto w = outputBlob->size[3];

    // find the position of the body parts
    std::vector<cv::Point> points(PoseParts);
    for (int n = 0; n < PoseParts; n++)
    {
        // Probability map of corresponding body's part.
        cv::Mat probMap(h, w, CV_32F, outputBlob->ptr(0, n));
        cv::Point2f p(-1, -1);
        cv::Point maxLoc;
        double prob;

        minMaxLoc(probMap, nullptr, &prob, nullptr, &maxLoc);

        if (prob > ThreshHold)
        {
            p = maxLoc;
            p.x *= Size.width / w;
            p.y *= Size.height / h;
        }
        points[n] = p;
    }

    const auto nPairs = static_cast<int>(PosePairs.size() / PosePairs[0].size());

    for (int n = 0; n < nPairs; n++)
    {
        // lookup 2 connected body/hand parts
        cv::Point2f partA = points[PosePairs[n][0]];
        cv::Point2f partB = points[PosePairs[n][1]];

        if (partA.x <= 0 || partA.y <= 0 || partB.x <= 0 || partB.y <= 0)
            continue;

        line(*mat, partA, partB, cv::Scalar(65, 115, 190), 6);
        circle(*mat, partA, 8, cv::Scalar(220, 220, 220), 1);
        circle(*mat, partB, 8, cv::Scalar(220, 220, 220), 1);
    }
}
