#include "stdafx.h"
#include "Gui.hpp"

//// Methods
//Gui::Gui() : Runnable(), FrontCameraCancellationToken(false), BackCameraCancellationToken(false)
//{
//    FrontCamera = std::make_unique<Camera>(Config::FrontCameraLinker, Config::FrontCameraSize, Config::CameraApi);
//    //BackCamera = std::make_unique<Camera>(config->BackCameraLinker, config->BackCameraSize, config->CameraApi);
//
//    Net = PoseEstimation::CreateDnnNet(Config::ProtoTextPath, Config::CaffeModel, Config::DnnMode);
//    FrontCameraEstimator = std::make_unique<PoseEstimation>(Net, Config::FrontCameraSize, Config::PoseParts, Config::PosePairs, Config::ThreshHold);
//    BackCameraEstimator = std::make_unique<PoseEstimation>(Net, Config::BackCameraSize, Config::PoseParts, Config::PosePairs, Config::ThreshHold);
//
//    InitCamera();
//}
//
//// Inside GUI render loop
//void Gui::Loop()
//{
//    //ImGui::ShowDemoWindow();
//
//    // Debug window
//	ImGui::Text("%.1f FPS", static_cast<double>(ImGui::GetIO().Framerate));
//    ImGui::Spacing();
//    ImGui::Checkbox("Pose estimation", &ShowPoseEstimation);
//    ImGui::Checkbox("3D model", &Show3dModel);
//    ImGui::Checkbox("3D model controls", &Show3dModelControls);
//
//    // Front camera window
//	ImGui::Begin("Front camera");
//    if (ShowPoseEstimation)
//    {
//        FrontCameraEstimator->Update(FrontCamera->GetMat());
//	    ImGui::Image(FrontCameraEstimator->GetTexture(), Config::FrontCameraSize);
//    }
//    else    
//    {        
//		FrontCamera->UpdateImage(); 
//	    ImGui::Image(FrontCamera->GetTexture(), Config::FrontCameraSize);
//    }
//    ImGui::End();
//
//	// Back camera window
//  //  ImGui::Begin("Back camera");
//  //  if(ShowPoseEstimation)
//  //  {
//  //      BackCameraEstimator->Update(BackCamera->GetMat());
//		//ImGui::Image(BackCameraEstimator->GetTexture(), Config::BackCameraSize);
//  //  }
//  //  else
//  //  {        
//		//BackCamera->UpdateImage();
//		//ImGui::Image(BackCamera->GetTexture(), Config::BackCameraSize);
//  //  }
//  //  ImGui::End();
//
//    // 3d model window
//    if (Show3dModel)
//    {
//		ImGui::Begin("3D model");
//
//    
//
//		ImGui::End();
//    }
//
//    // 3d model controls window
//    if (Show3dModelControls)
//    {
//        //ImGui::Begin("3D model controls");
//
//        //ImGui::SliderInt("Fov", Model->FovPtr(), 20, 90);
//        //ImGui::SliderInt("Pitch", Model->PitchPtr(), -180, 180);
//        //ImGui::SliderInt("Yaw", Model->YawPtr(), -180, 180);
//        //ImGui::SliderInt("Roll", Model->RollPtr(), -180, 180);
//        //ImGui::SliderFloat("Distance", Model->DistancePtr(), 0.0f, 30.0f);
//        //ImGui::SliderFloat3("Focus", Model->FocusPtr(), -2.0f, 2.0f);
//        //ImGui::SliderFloat3("Light position", Model->LightPositionPtr(), -20.0f, 20.0f);
//
//        //ImGui::Spacing();
//        //if (ImGui::Button("Reset"))
//        //    Model.SetToDefault();
//
//        //ImGui::End();
//    }
//}
//
//void Gui::HandleInput()
//{
//  //  double x, y;
//  //  glfwGetCursorPos(Window, &x, &y);
//
//  //  // Model input
//  //  if(Show3dModel)
//		//Model->OnMouseMove(x, y, Input::GetPressedButton(Window));
//}
//
//void Gui::InitCamera() const
//{
//    if (!FrontCamera->OpenCamera())
//        throw std::exception("Front camera error");
//    //if (!BackCamera->OpenCamera())
//    //    throw std::exception("Back camera error");
//
//    FrontCamera->SetUpdateCameraThread(FrontCameraUpdateThread, FrontCameraCancellationToken);
//    /*BackCamera->SetUpdateCameraThread(BackCameraUpdateThread, BackCameraCancellationToken);*/
//}
//
//void Gui::ShutdownCamera()
//{
//    FrontCameraCancellationToken = true;
//    //BackCameraCancellationToken = true;
//}
//
//Gui::~Gui()
//{
//    ShutdownCamera();
//    Shutdown();
//}