//
// Created by matt on 2020-07-26.
//

#include "IMGUI_Editor.h"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace dogb::IMGUI
{
void
EditorDrawTransform(glm::mat4 &value)
{
    bool modified = false;

    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;

    // separate out the important data in the matrix
    glm::decompose(value, scale, rotation, translation, skew, perspective);

    glm::vec3 euler_rotation = glm::eulerAngles(rotation);

    if (ImGui::DragFloat3("Position", glm::value_ptr(translation)))
        modified |= true;
    if (ImGui::DragFloat3("Rotation", glm::value_ptr(euler_rotation)))
        modified |= true;
    if (ImGui::DragFloat3("Scale", glm::value_ptr(scale)))
        modified |= true;

    if (modified)
    {
        auto quat = glm::toQuat(glm::orientate3(euler_rotation));

        // Create back our transform matrix
        value = glm::translate(glm::mat4(), translation) *
              glm::toMat4(quat) * glm::scale(glm::mat4(), scale);
    }
}
} // namespace dogb::IMGUI