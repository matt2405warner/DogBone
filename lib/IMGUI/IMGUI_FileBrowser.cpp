//
// Created by matt on 2020-08-10.
//

#include "IMGUI_FileBrowser.h"

#include "IMGUI_Std.h"
#include "IMGUI_SubSystem.h"

#include <imgui.h>

#include <filesystem>
#include <utility>

namespace fs = std::filesystem;

namespace dogb::IMGUI
{
FileBrowser::FileBrowser(const std::string &path, SelectClb clb)
    : m_selectClb(std::move(clb))
    , m_currentDirectory(path)
    , m_currentSelection("")
    , m_width(500)
    , m_height(500)
{
    setTitle("File Browser");
}

void
FileBrowser::open()
{
    m_open = true;

    ImGui::OpenPopup(m_openLabel.c_str());
}

void
FileBrowser::close()
{
    m_open = false;
}

void
FileBrowser::setTitle(const std::string &title)
{
    m_title = title;
    m_openLabel = m_title + "##filebrowser_" +
                  std::to_string(reinterpret_cast<size_t>(this));
}
FileBrowser *
FileBrowser::openBrowser(const std::string &directory, SelectClb clb)
{
    std::unique_ptr<FileBrowser> browser(
            new FileBrowser(directory, std::move(clb)));
    browser->open();

    UT::Engine &engine = UT::Engine::get();
    IMGUI::SubSystem *sys = engine.getOrCreateSubSystem<IMGUI::SubSystem>();
    return static_cast<FileBrowser *>(sys->addModal(std::move(browser)));
}

void
FileBrowser::render()
{
    if (!m_open)
        return;

    if (!ImGui::IsPopupOpen(m_openLabel.c_str()))
        ImGui::OpenPopup(m_openLabel.c_str());

    constexpr int modal_flags =
                                ImGuiWindowFlags_NoCollapse |
                                ImGuiWindowFlags_NoScrollbar |
                                ImGuiWindowFlags_AlwaysAutoResize;
    if (!ImGui::BeginPopupModal(m_openLabel.c_str(), &m_open, modal_flags))
    {
        return;
    }

    ImGui::PushItemWidth(-1);
    ImGui::Text("Directory:");
    ImGui::SameLine();
    IMGUI::InputText("", m_currentDirectory);

    if (ImGui::ListBoxHeader(""))
    {
        if (ImGui::Selectable(".."))
        {
            m_currentDirectory = fs::path(m_currentDirectory).parent_path();
            m_currentSelection.clear();
        }

        for (auto &p : fs::directory_iterator(m_currentDirectory))
        {
            if (!matchesFilter(p.path().generic_string()))
                continue;

            if (ImGui::Selectable(p.path().filename().c_str()))
            {
                if (p.is_directory())
                {
                    m_currentSelection.clear();
                    m_currentDirectory = p.path().generic_string();
                }
                else
                {
                    m_currentSelection = p.path().generic_string();
                }
            }
        }

        ImGui::ListBoxFooter();
    }
    ImGui::PopItemWidth();

    ImGui::Dummy(ImVec2(0.0, 10.0f));

    if (ImGui::Button("Select"))
    {
        if (m_selectClb)
            m_selectClb(m_currentSelection);

        m_open = false;
    }
    ImGui::SameLine();
    IMGUI::InputText("", m_currentSelection);

    if (!m_filters.empty() && m_currentFilter < m_filters.size())
    {
        ImGui::SameLine();
        IMGUI::ComboBox("##filter", m_currentFilter, m_filters);
    }

    ImGui::EndPopup();
}
bool
FileBrowser::matchesFilter(const std::filesystem::path &path) const
{
    if (m_filters.empty())
        return true;

    if (m_currentFilter >= m_filters.size())
        return false;

    if (fs::is_directory(path))
        return true;

    return m_filters[m_currentFilter] == path.extension();
}

} // namespace dogb::IMGUI
