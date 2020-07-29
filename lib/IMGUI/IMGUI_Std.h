//
// Created by matt on 2020-07-28.
//

#ifndef DOGBONE_IMGUI_STD_H
#define DOGBONE_IMGUI_STD_H

#include "IMGUI_API.h"

#include <imgui.h>

#include <string>

namespace dogb::IMGUI
{
DB_IMGUI_API bool InputText(
        const char *label,
        std::string &str,
        ImGuiInputTextFlags flags = 0,
        ImGuiInputTextCallback callback = nullptr,
        void *user_data = nullptr);
DB_IMGUI_API bool InputTextMultiline(
        const char *label,
        std::string &name,
        const ImVec2 &size = ImVec2(0, 0),
        ImGuiInputTextFlags flags = 0,
        ImGuiInputTextCallback callback = nullptr,
        void *user_data = nullptr);
DB_IMGUI_API bool InputTextWithHint(
        const char *label,
        const char *hint,
        std::string &str,
        ImGuiInputTextFlags = 0,
        ImGuiInputTextCallback callback = nullptr,
        void *user_data = nullptr);
} // namespace dogb::IMGUI

#endif // DOGBONE_IMGUI_STD_H
