//
// Created by matt on 2020-07-28.
//

#include "IMGUI_Std.h"

#include <UT/UT_Assert.h>

namespace dogb::IMGUI
{
struct InputTextCallback_UserData
{
    std::string *Str;
    ImGuiInputTextCallback ChainCallback;
    void *ChainCallbackUserData;
};

static int
InputTextCallback(ImGuiInputTextCallbackData *data)
{
    InputTextCallback_UserData *user_data =
            (InputTextCallback_UserData *)data->UserData;
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
    {
        // Resize string callback
        // If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
        std::string *str = user_data->Str;
        UT_ASSERT(data->Buf == str->c_str());
        str->resize(static_cast<unsigned long>(data->BufTextLen));
        data->Buf = (char *)str->c_str();
    }
    else if (user_data->ChainCallback)
    {
        // Forward to user callback, if any
        data->UserData = user_data->ChainCallbackUserData;
        return user_data->ChainCallback(data);
    }
    return 0;
}

bool
InputText(
        const char *label,
        std::string &str,
        ImGuiInputTextFlags flags,
        ImGuiInputTextCallback callback,
        void *user_data)
{
    UT_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
    flags |= ImGuiInputTextFlags_CallbackResize;

    InputTextCallback_UserData cb_user_data;
    cb_user_data.Str = &str;
    cb_user_data.ChainCallback = callback;
    cb_user_data.ChainCallbackUserData = user_data;
    return ImGui::InputText(
            label, (char *)str.c_str(), str.capacity() + 1, flags,
            InputTextCallback, &cb_user_data);
}

bool
InputTextMultiline(
        const char *label,
        std::string &str,
        const ImVec2 &size,
        ImGuiInputTextFlags flags,
        ImGuiInputTextCallback callback,
        void *user_data)
{
    UT_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
    flags |= ImGuiInputTextFlags_CallbackResize;

    InputTextCallback_UserData cb_user_data;
    cb_user_data.Str = &str;
    cb_user_data.ChainCallback = callback;
    cb_user_data.ChainCallbackUserData = user_data;
    return ImGui::InputTextMultiline(
            label, (char *)str.c_str(), str.capacity() + 1, size, flags,
            InputTextCallback, &cb_user_data);
}

bool
InputTextWithHint(
        const char *label,
        const char *hint,
        std::string &str,
        ImGuiInputTextFlags flags,
        ImGuiInputTextCallback callback,
        void *user_data)
{
    UT_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
    flags |= ImGuiInputTextFlags_CallbackResize;

    InputTextCallback_UserData cb_user_data;
    cb_user_data.Str = &str;
    cb_user_data.ChainCallback = callback;
    cb_user_data.ChainCallbackUserData = user_data;
    return ImGui::InputTextWithHint(
            label, hint, (char *)str.c_str(), str.capacity() + 1, flags,
            InputTextCallback, &cb_user_data);
}

bool
ComboBox(
        const char* label,
        std::vector<std::string>::size_type & current_index,
        const std::vector<std::string>& values)
{
    if (current_index >= values.size() || values.empty())
        return false;

    if (!ImGui::BeginCombo(label, values[current_index].c_str()))
    {
        return false;
    }

    std::vector<std::string>::size_type idx = 0;
    for (auto&& v : values)
    {
        if (ImGui::Selectable(v.c_str(), idx == current_index))
        {
            current_index = idx;
        }
        idx++;
    }

    ImGui::EndCombo();

    return true;
}
}