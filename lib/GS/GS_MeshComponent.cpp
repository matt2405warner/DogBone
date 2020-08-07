//
// Created by matt on 2020-08-04.
//

#include "GS_MeshComponent.h"

#include <imgui.h>

namespace dogb::GS
{
void
MeshComponent::onGUI()
{
    if (!m_mesh.m_material)
        return;

    GS::Material& _mat = *m_mesh.m_material;
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

        ImGui::Text("Main Texture:");
        ImGui::Image(
                reinterpret_cast<void *>(tex_id),
                viewport_size, ImVec2{0, 1},
                ImVec2{1, 0});
    }
}
} // namespace dogb::GS