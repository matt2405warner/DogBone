//
// Created by matt on 2020-07-26.
//

#include "IMGUI_Editor.h"

#include "IMGUI_StandardTypes.h"

#include <glm/gtc/type_ptr.hpp>

namespace dogb::IMGUI
{
static int imgui_VariantToInt32(rttr::variant& v, int _default)
{
    int result = _default;
    if (v.is_valid())
    {
        bool ok = false;
        int value = v.to_int(&ok);
        if (ok)
            result = value;
    }
    return result;
}
static float imgui_VariantToFloat(rttr::variant& v, float _default)
{
    float result = _default;
    if (v.is_valid())
    {
        bool ok = false;
        float value = v.to_float(&ok);
        if (ok)
            result = value;
    }
    return result;
}

void
Editor::initialize()
{
    theImpl = std::make_unique<dogb::IMGUI::Editor>();

    auto editor = static_cast<IMGUI::Editor*>(theImpl.get());
    editor->m_typeCallbacks[rttr::type::get<int>().get_id()] = &drawInt32Clb;
    editor->m_typeCallbacks[rttr::type::get<glm::vec3>().get_id()] = &drawGLMVec3;
    editor->m_typeCallbacks[rttr::type::get<glm::vec4>().get_id()] = &drawGLMVec4;

    // Register our extra types
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(glm::vec2);
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(glm::vec3);
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(glm::vec4);
}

void
Editor::drawInt32Clb(rttr::instance& _obj, const rttr::type& t, const rttr::property& prop)
{
    // For some reason using numeric limits causes imgui to freak out so we use
    // completely arbitrary values here.
    int min = -1000;
    int max = 1000;

    if (prop.is_valid())
    {
        auto min_meta = prop.get_metadata("min_value");
        auto max_meta = prop.get_metadata("max_value");
        min = imgui_VariantToInt32(min_meta, min);
        max = imgui_VariantToInt32(max_meta, max);

        int32_t i = prop.get_value(_obj).to_int32();
        rttr::string_view view = prop.get_name();
        if (ImGui::SliderInt(view.begin(), &i, min, max))
            prop.set_value(_obj, i);
    }
    else if (t.is_valid())
    {
        auto min_meta = t.get_metadata("min_value");
        auto max_meta = t.get_metadata("max_value");
        min = imgui_VariantToInt32(min_meta, min);
        max = imgui_VariantToInt32(max_meta, max);

        int32_t* i = _obj.try_convert<int32_t>();
        ImGui::SliderInt("int", i, min, max);
    }
}

void
Editor::drawGLMVec3(rttr::instance& _obj, const rttr::type& t, const rttr::property& prop)
{
    // For some reason using numeric limits causes imgui to freak out so we use
    // completely arbitrary values here.
    float min = -1000;
    float max = 1000;

    if (prop.is_valid())
    {
        auto min_meta = prop.get_metadata("min_value");
        auto max_meta = prop.get_metadata("max_value");
        min = imgui_VariantToFloat(min_meta, min);
        max = imgui_VariantToFloat(max_meta, max);

        bool modified = false;
        glm::vec3 v = prop.get_value(_obj).convert<glm::vec3>();
        rttr::string_view view = prop.get_name();

        ImGui::Text("%s:", view.begin());
        if (ImGui::SliderFloat( "x", &(v.x), min, max))
            modified |= true;
        if (ImGui::SliderFloat("y", &(v.y), min, max))
            modified |= true;
        if (ImGui::SliderFloat("z", &(v.z), min, max))
            modified |= true;

        if (modified)
            prop.set_value(_obj, v);
    }
    else if (t.is_valid())
    {
        glm::vec3* v = _obj.try_convert<glm::vec3>();
        ImGui::Text("%s:", t.get_name().begin());
        ImGui::SliderFloat("x", &(v->x), min, max);
        ImGui::SliderFloat("y", &(v->y), min, max);
        ImGui::SliderFloat("z", &(v->z),min, max);
    }
}

void
Editor::drawGLMVec4(rttr::instance& _obj, const rttr::type& t, const rttr::property& prop)
{
    // For some reason using numeric limits causes imgui to freak out so we use
    // completely arbitrary values here.
    float min = -1000;
    float max = 1000;

    if (prop.is_valid())
    {
        auto min_meta = prop.get_metadata("min_value");
        auto max_meta = prop.get_metadata("max_value");
        min = imgui_VariantToFloat(min_meta, min);
        max = imgui_VariantToFloat(max_meta, max);

        bool modified = false;
        glm::vec4 v = prop.get_value(_obj).convert<glm::vec4>();
        rttr::string_view view = prop.get_name();

        ImGui::Text("%s:", view.begin());
        if (ImGui::SliderFloat( "x", &(v.x), min, max))
            modified |= true;
        if (ImGui::SliderFloat("y", &(v.y), min, max))
            modified |= true;
        if (ImGui::SliderFloat("z", &(v.z), min, max))
            modified |= true;
        if (ImGui::SliderFloat("w", &(v.w), min, max))
            modified |= true;

        if (modified)
            prop.set_value(_obj, v);
    }
    else if (t.is_valid())
    {
        glm::vec4* v = _obj.try_convert<glm::vec4>();
        ImGui::Text("%s:", t.get_name().begin());
        ImGui::SliderFloat("x", &(v->x), min, max);
        ImGui::SliderFloat("y", &(v->y), min, max);
        ImGui::SliderFloat("z", &(v->z),min, max);
        ImGui::SliderFloat("w", &(v->w), min, max);
    }
}

}