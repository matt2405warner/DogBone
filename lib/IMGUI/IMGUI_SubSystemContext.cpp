//
// Created by matt on 2020-07-11.
//

#include "IMGUI_SubSystemContext.h"

#include "IMGUI_GLFWImpl.h"
#include "IMGUI_OpenGLImpl.h"

#include <CE/CE_Input.h>

#include <UT/UT_Window.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <ImGuizmo.h>

namespace dogb::IMGUI
{
SubSystemContext::SubSystemContext(UT::Window *window)
    : m_window(window), m_imguiContext(nullptr)
{
}

SubSystemContext::~SubSystemContext()
{
    if (m_imguiContext)
        onShutdown();
}

void
SubSystemContext::onShutdown()
{
    for (auto&& win : m_windows)
        win->onDestroy();
    m_windows.clear();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(m_imguiContext);
    m_imguiContext = nullptr;
}

void
SubSystemContext::onAttach()
{
#if 0
    m_dockspaceInitialized = false;
#endif

    IMGUI_CHECKVERSION();
    m_imguiContext = ImGui::CreateContext();

    initFonts();
    setDarkTheme();

    GLFWwindow *native_window =
            static_cast<GLFWwindow *>(m_window->nativeWindow());

    ImGui_ImplOpenGL3_Init("#version 410");
    ImGui_ImplGlfw_InitForOpenGL(native_window, true);

    ImGuiIO &io = ImGui::GetIO();
    io.DisplaySize = ImVec2(
            static_cast<float>(m_window->width()),
            static_cast<float>(m_window->height()));
    io.DeltaTime = 1.0f / 60.0f;

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    io.ConfigDockingWithShift = false;

    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

    using namespace CE::Input;
    io.KeyMap[ImGuiKey_Tab] = KEY_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow] = KEY_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = KEY_PAGE_UP;
    io.KeyMap[ImGuiKey_PageDown] = KEY_PAGE_DOWN;
    io.KeyMap[ImGuiKey_Home] = KEY_HOME;
    io.KeyMap[ImGuiKey_End] = KEY_END;
    io.KeyMap[ImGuiKey_Insert] = KEY_INSERT;
    io.KeyMap[ImGuiKey_Delete] = KEY_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Space] = KEY_SPACE;
    io.KeyMap[ImGuiKey_Enter] = KEY_ENTER;
    io.KeyMap[ImGuiKey_Escape] = KEY_ESCAPE;
    io.KeyMap[ImGuiKey_A] = KEY_A;
    io.KeyMap[ImGuiKey_C] = KEY_C;
    io.KeyMap[ImGuiKey_V] = KEY_V;
    io.KeyMap[ImGuiKey_X] = KEY_X;
    io.KeyMap[ImGuiKey_Y] = KEY_Y;
    io.KeyMap[ImGuiKey_Z] = KEY_Z;
}

ImGuiID
SubSystemContext::getDockID(IMGUI::Window::DockDirection dir)
{
    ImGuiID dock_id = m_dockspaceID;

    switch (dir)
    {
    case IMGUI::Window::DockDirection::DockLeft:
        dock_id = m_dockLeftID;
        break;
    case IMGUI::Window::DockDirection::DockRight:
        dock_id = m_dockRightID;
        break;
    case IMGUI::Window::DockDirection::DockUp:
        dock_id = m_dockUpID;
        break;
    case IMGUI::Window::DockDirection::DockDown:
        dock_id = m_dockDownID;
        break;
    default:
        break;
    };

    return dock_id;
}

void SubSystemContext::update(UT::Timestep ts)
{
    ImGui::SetCurrentContext(m_imguiContext);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    onGUI();

    ImGuiIO &io = ImGui::GetIO();
    float width = static_cast<float>(m_window->width());
    float height = static_cast<float>(m_window->height());
    io.DisplaySize = ImVec2(width, height);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
    window_flags |= ImGuiWindowFlags_NoDocking;
    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::SetNextWindowBgAlpha(0.0f);

    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                    ImGuiWindowFlags_NoBringToFrontOnFocus |
                    ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("DockspaceWindow", nullptr, window_flags);
    ImGui::PopStyleVar(3);

    ImGuiStyle& style = ImGui::GetStyle();
    float min_window_size = style.WindowMinSize.x;
    style.WindowMinSize.x = 370.0f;

    m_dockspaceID = ImGui::GetID("MyDockspace");
    ImGui::DockSpace(
            m_dockspaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

    style.WindowMinSize.x = min_window_size;
    ImGui::End();

    if (!m_dockspaceInitialized)
    {
        m_dockspaceInitialized = true;
        ImGui::DockBuilderRemoveNode(m_dockspaceID);
        ImGui::DockBuilderAddNode(m_dockspaceID, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(m_dockspaceID, viewport->Size);

        m_dockLeftID = ImGui::DockBuilderSplitNode(
                m_dockspaceID, ImGuiDir_Left, 0.20f, NULL, &m_dockspaceID);
        m_dockRightID = ImGui::DockBuilderSplitNode(
                m_dockspaceID, ImGuiDir_Right, 0.20f, NULL, &m_dockspaceID);
        m_dockUpID = ImGui::DockBuilderSplitNode(
                m_dockspaceID, ImGuiDir_Up, 0.20f, NULL, &m_dockspaceID);
        m_dockDownID = ImGui::DockBuilderSplitNode(
                m_dockspaceID, ImGuiDir_Down, 0.20f, NULL, &m_dockspaceID);
        ImGui::DockBuilderFinish(m_dockspaceID);
    }
    for (std::unique_ptr<IMGUI::Window> &win : m_windows)
    {
        if (!win->isOpen())
            continue;
        ImGuiID dock_id = getDockID(win->m_dockDirection);

        win->onPreGUI();
        ImGui::Begin(win->title().c_str(), &win->m_isOpen);
        if (!win->m_initializedDock)
        {
            win->m_initializedDock = true;
            ImGui::DockBuilderDockWindow(win->title().c_str(), dock_id);
        }
        win->onGUI(ts);

        ImGui::End();

        win->onPostGUI();
    }

    renderModals();

    ImGui::EndFrame();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow * tmp_ctx = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(tmp_ctx);
    }
}

void
SubSystemContext::initFonts()
{
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("../assets/fonts/OpenSans-Bold.ttf", 14.0f);
    io.FontDefault = io.Fonts->AddFontFromFileTTF("../assets/fonts/OpenSans-Regular.ttf", 14.0f);
}

void
SubSystemContext::setDarkTheme()
{
    ImGui::StyleColorsDark();

    ImGuiStyle &style = ImGui::GetStyle();

    style.WindowRounding = 0.0f;
    style.ChildRounding = 0.0f;
    style.FrameRounding = 0.0f;
    style.GrabRounding = 0.0f;
    style.ScrollbarRounding = 0.0f;

    auto& colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

    // Headers
    colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Buttons
    colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
    colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
    colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
}

Modal*
SubSystemContext::addModal(std::unique_ptr<Modal> modal)
{
    return m_modals.emplace_back(std::move(modal)).get();
}

void
SubSystemContext::renderModals()
{
    // Render and clear out all the old modals
    for (auto it = m_modals.begin(); it != m_modals.end(); )
    {
        std::unique_ptr<Modal>& modal = (*it);
        if (!modal)
        {
            it = m_modals.erase(it);
            continue;
        }

        modal->render();

        if (!modal->isOpen())
        {
            it = m_modals.erase(it);
        }
        else
            it++;
    }
}

} // namespace dogb::IMGUI
