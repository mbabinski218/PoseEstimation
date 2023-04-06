#include "Demo.hpp"

#include "Loader.hpp"
#include "Model.hpp"

// Methods
Demo::Demo() : Runnable()
{
    DemoWorld.AddModel(1);
    DemoWorld.AddModel(2);

    auto model = DemoWorld.GetModel(1);
    Loader::LoadModel(Config::ModelObjPath, *model);
	model->Animate(Animation::Create(model->GetBoneInfoMap()));
}

// Inside GUI render loop
void Demo::Loop()
{
    //ImGui::ShowDemoWindow();

    // DeltaTime
    const auto currentFrame = static_cast<float>(glfwGetTime());
    DeltaTime = currentFrame - LastFrame;
    LastFrame = currentFrame;

    // Debug window
    ImGui::Text("%.1f FPS", static_cast<double>(ImGui::GetIO().Framerate));
    ImGui::Spacing();
    ImGui::Checkbox("3D model", &Show3dModel);
    ImGui::Checkbox("3D model controls", &Show3dModelControls);

    // 3d model window
    if (Show3dModel)
    {
        ImGui::Begin("3D model");

        const auto windowSize = ImGui::GetWindowSize();
        DemoWorld.Update(windowSize, DeltaTime);
        DemoWorld.Draw();
        ImGui::Image(DemoWorld.GetTexture(), ImVec2(windowSize.x - 2.0f * Padding.x, windowSize.y - 4.5f * Padding.y));

        ImGui::End();
    }

    // 3d model controls window
    //if (Show3dModelControls)
    //{
    //    ImGui::Begin("3D model controls");

    //    // Camera
    //    ImGui::Spacing();
    //    ImGui::Text("Camera");
    //    ImGui::SliderInt("Fov", Model->FovPtr(), 20, 90);
    //    ImGui::SliderInt("Pitch", Model->PitchPtr(), -180, 180);
    //    ImGui::SliderInt("Yaw", Model->YawPtr(), -180, 180);
    //    ImGui::SliderInt("Roll", Model->RollPtr(), -180, 180);
    //    ImGui::SliderFloat("Distance", Model->DistancePtr(), 0.0f, 30.0f);
    //    ImGui::SliderFloat3("Focus", Model->FocusPtr(), -2.0f, 2.0f);

    //    // Light
    //    ImGui::Spacing();
    //    ImGui::Text("Light");
    //    ImGui::SliderFloat3("Position", Model->LightPositionPtr(), -20.0f, 20.0f);

    //    // Model
    //    ImGui::Spacing();
    //    ImGui::Text("Model");
    //    ImGui::SliderFloat3("Rotation", Model->RotationPtr(), -180.0f, 180.0f);
    //    ImGui::SliderFloat3("Scale", Model->ScalePtr(), 0.0f, 5.0f);

    //    // Reset
    //    ImGui::Spacing();
    //    if (ImGui::Button("Reset"))
    //        Model->Reset();

    //    ImGui::End();
    //}
}

void Demo::HandleInput()
{
    double x, y;
    glfwGetCursorPos(Window, &x, &y);

    // Model input
    if (Show3dModel)
        DemoWorld.GetView()->OnMouseMove(x, y, Input::GetPressedButton(Window));
}

Demo::~Demo()
{
    Shutdown();
}