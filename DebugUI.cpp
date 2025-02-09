#include "DebugUI.h"

std::vector<std::function<void(void)>> DebugUI::m_debugfunction;

void DebugUI::Init(ID3D11Device* device, ID3D11DeviceContext* context) 
{

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(Application::GetWindow());
    ImGui_ImplDX11_Init(device, context);
}

void DebugUI::DisposeUI() {
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

// �f�o�b�O�\���֐��̓o�^
void DebugUI::RedistDebugFunction(std::function<void(void)> f) {
    m_debugfunction.push_back(std::move(f));
}

void DebugUI::Render() {
    // ImGui�̐V�����t���[�����J�n
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // �E�B���h�E�ƃf�o�b�O���̕`��
    ImGui::Begin("Debug Information");
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

    ImGui::End();

    // �f�o�b�O�֐��̎��s
    for (auto& f : m_debugfunction)
    {
        f();
    }

    // �t���[���̃����_�����O������
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
