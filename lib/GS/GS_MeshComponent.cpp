//
// Created by matt on 2020-08-04.
//

#include "GS_MeshComponent.h"

#include <IMGUI/IMGUI_FileBrowser.h>

#include <imgui.h>

namespace dogb::GS
{
void
MeshComponent::onGUI()
{
    if (!m_mesh.m_material)
        return;

    GS::Material &_mat = *m_mesh.m_material;
    if (_mat.m_mainTexture)
    {
        uint32_t tex_id = _mat.m_mainTexture->renderId();

        ImVec2 viewport_size = ImGui::GetContentRegionAvail();

        static constexpr int min_size = 40;
        static constexpr int max_size = 100;
        if (viewport_size.x < min_size)
            viewport_size.x = min_size;
        else if (viewport_size.x > max_size)
            viewport_size.x = max_size;

        viewport_size.y = viewport_size.x;

        if (ImGui::Button("File Browser"))
        {
            IMGUI::FileBrowser* browser = IMGUI::FileBrowser::openBrowser(
                    "/home/matt/", [this](const std::string &selected_file) {
                        selectFileCallback_(selected_file);
                    });
            browser->setFilters({".png", ".jpg", ".jpeg"});
        }

        ImGui::Text("Main Texture:");
        ImGui::Image(
                reinterpret_cast<void *>(tex_id), viewport_size, ImVec2{0, 1},
                ImVec2{1, 0});
    }
}
void
MeshComponent::selectFileCallback_(const std::string &file)
{
    m_mesh.m_material->m_mainTexture = GR::Texture2D::create(file);
}

} // namespace dogb::GS