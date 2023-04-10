#include "stdafx.h"
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

void PoseEstimation::FindPose(const cv::Mat& mat)
{
	const auto inputBlob = cv::dnn::blobFromImage(mat, 1.0 / 255.0, cv::Size(299, 299), cv::Scalar(0, 0, 0));
	Net->setInput(inputBlob);
    OutputBlob = std::make_unique<cv::Mat>(Net->forward());
}

void PoseEstimation::Update(const cv::Mat mat, World& world)
{
    FindPose(mat);
    AddPoseToImage(mat);

    auto updateWorldThread = std::jthread([&] { UpdateWorldModels(world); });
    Image.UpdateMat(mat);
}

void* PoseEstimation::GetTexture() const
{
	return Image.GetTexture();
}

void PoseEstimation::AddPoseToImage(const cv::Mat& mat)
{
    const auto height = OutputBlob->size[2];
    const auto width = OutputBlob->size[3];
    auto points = std::vector<cv::Point>(Config::PoseParts);

    PoseSkeleton.MoveAndClear();

    for (int n = 0; n < Config::PoseParts; n++)
    {
        const auto probMap = cv::Mat(height, width, CV_32F, OutputBlob->ptr(0, n));
        auto point = cv::Point2f(-1, -1);
        auto maxLoc = cv::Point{};
        auto maxVal = 0.0;

        minMaxLoc(probMap, nullptr, &maxVal, nullptr, &maxLoc);

        if (maxVal > Config::ThreshHold)
        {
            point = maxLoc;
            point.x *= CameraSize.width / static_cast<float>(width);
            point.y *= CameraSize.height / static_cast<float>(height);
        }
        points[n] = point;
    }

    for (const auto& posePair : Config::PosePairs)
    {
        cv::Point2f partA = points[posePair[0]];
        cv::Point2f partB = points[posePair[1]];

        if (partA.x <= 0 || partA.y <= 0 || partB.x <= 0 || partB.y <= 0)
            continue;

        PoseSkeleton.Current.try_emplace(posePair[0], partA);
        PoseSkeleton.Current.try_emplace(posePair[1], partB);

        line(mat, partA, partB, cv::Scalar(65, 115, 190), 5);
        circle(mat, partA, 4, cv::Scalar(220, 220, 220), 1);
        circle(mat, partB, 4, cv::Scalar(220, 220, 220), 1);
        putText(mat, std::to_string(posePair[0]), partA, 0, 0.5, cv::Scalar(0, 0, 0), 2);
        putText(mat, std::to_string(posePair[1]), partB, 0, 0.5, cv::Scalar(0, 0, 0), 2);
    }
}

void PoseEstimation::UpdateWorldModels(World& world) const
{
    const auto model = world.GetModel(1);
    const auto animation = Animation(PoseSkeleton, model->GetBoneInfoMap());
    model->Animate(std::make_shared<Animation>(animation));
}