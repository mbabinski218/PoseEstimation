#include "Gui.hpp"

// Methods
Gui::Gui() : Runnable(), FrontCameraCancellationToken(false), BackCameraCancellationToken(false)
{
    FrontCamera = std::make_unique<Camera>(GuiConfig->FrontCameraLinker, GuiConfig->FrontCameraSize, GuiConfig->CameraApi);
    //BackCamera = std::make_unique<Camera>(config->BackCameraLinker, config->BackCameraSize, config->CameraApi);

    Net = PoseEstimation::CreateDnnNet(GuiConfig->ProtoTextPath, GuiConfig->CaffeModel, GuiConfig->DnnMode);
    FrontCameraEstimator = std::make_unique<PoseEstimation>(Net, GuiConfig->FrontCameraSize, GuiConfig->PoseParts, GuiConfig->PosePairs, GuiConfig->ThreshHold);
    BackCameraEstimator = std::make_unique<PoseEstimation>(Net, GuiConfig->BackCameraSize, GuiConfig->PoseParts, GuiConfig->PosePairs, GuiConfig->ThreshHold);

    Model = std::make_unique<Mesh>(GuiConfig->ModelObjPath, GuiConfig->VertexCorePath, GuiConfig->FragmentCorePath);

    Padding = ImGui::GetStyle().WindowPadding;

    InitCamera();
}

// Inside GUI render loop
void Gui::Loop() const
{
    //ImGui::ShowDemoWindow();

    // Debug window
	ImGui::Text("%.1f FPS", static_cast<double>(ImGui::GetIO().Framerate));
    ImGui::Spacing();
    ImGui::Checkbox("Pose estimation", &ShowPoseEstimation);
    ImGui::Checkbox("3D model", &Show3dModel);
    ImGui::Checkbox("3D model controls", &Show3dModelControls);

    // Front camera window
	ImGui::Begin("Front camera");
    if (ShowPoseEstimation)
    {
        FrontCameraEstimator->Update(FrontCamera->GetMat());
	    ImGui::Image(FrontCameraEstimator->GetTexture(), GuiConfig->FrontCameraSize);
    }
    else    
    {        
		FrontCamera->UpdateImage(); 
	    ImGui::Image(FrontCamera->GetTexture(), GuiConfig->FrontCameraSize);
    }
    ImGui::End();

	// Back camera window
  //  ImGui::Begin("Back camera");
  //  if(ShowPoseEstimation)
  //  {
  //      BackCameraEstimator->Update(BackCamera->GetMat());
		//ImGui::Image(BackCameraEstimator->GetTexture(), GuiConfig->BackCameraSize);
  //  }
  //  else
  //  {        
		//BackCamera->UpdateImage();
		//ImGui::Image(BackCamera->GetTexture(), GuiConfig->BackCameraSize);
  //  }
  //  ImGui::End();

    // 3d model window
    if (Show3dModel)
    {
		ImGui::Begin("3D model");

        const auto windowSize = ImGui::GetWindowSize();        
        Model->Update(windowSize);
        Model->Render();
        ImGui::Image(Model->GetTexture(), ImVec2(windowSize.x - 2.0f * Padding.x, windowSize.y - 4.5f * Padding.y));

		ImGui::End();
    }

    // 3d model controls window
    if (Show3dModelControls)
    {
        ImGui::Begin("3D model controls");

        ImGui::SliderInt("Fov", Model->FovPtr(), 20, 90);
        ImGui::SliderInt("Pitch", Model->PitchPtr(), -180, 180);
        ImGui::SliderInt("Yaw", Model->YawPtr(), -180, 180);
        ImGui::SliderInt("Roll", Model->RollPtr(), -180, 180);
        ImGui::SliderFloat("Distance", Model->DistancePtr(), 0.0f, 30.0f);
        ImGui::SliderFloat3("Focus", Model->FocusPtr(), -2.0f, 2.0f);
        ImGui::SliderFloat3("Light position", Model->LightPositionPtr(), -20.0f, 20.0f);

        ImGui::Spacing();
        if (ImGui::Button("Reset"))
            Model->Reset();

        ImGui::End();
    }
}

void Gui::HandleInput() const
{
    double x, y;
    glfwGetCursorPos(Window, &x, &y);

    // Model input
    if(Show3dModel)
		Model->OnMouseMove(x, y, Input::GetPressedButton(Window));
}

void Gui::InitCamera() const
{
    if (!FrontCamera->OpenCamera())
        throw std::exception("Front camera error");
    //if (!BackCamera->OpenCamera())
    //    throw std::exception("Back camera error");

    FrontCamera->SetUpdateCameraThread(FrontCameraUpdateThread, FrontCameraCancellationToken);
    /*BackCamera->SetUpdateCameraThread(BackCameraUpdateThread, BackCameraCancellationToken);*/
}

void Gui::ShutdownCamera()
{
    FrontCameraCancellationToken = true;
    //BackCameraCancellationToken = true;
}

Gui::~Gui()
{
    ShutdownCamera();
    Shutdown();
}