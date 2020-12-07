//
// Created by matt on 2020-12-07.
//

#include "UT_FileDialog.h"

#include <fmt/format.h>

namespace dogb::UT
{
std::optional<std::string>
openFile(const std::string &title, const std::string &filter)
{
#ifdef DB_LINUX
    static constexpr int size = 2048;
    char buf[size];

    std::string file_filter = filter;
    if (filter.empty())
        file_filter = "All files | *";

    std::string cmd = fmt::format(
            "zenity --file-selection --title=\"{}\" --file-filter='{}'", title, file_filter);

    FILE *f = popen(cmd.c_str(), "r");
    fgets(buf, size, f);
    int ret = pclose(f);

    if (ret < 0)
        return std::nullopt;

    std::string output(buf);
    std::string_view view(output);
    auto index = view.find_first_of('\n');
    if (index != std::string_view::npos)
        output.resize(index);

    return output;
#else
#error "Platform does not support open file..."
#endif
}

std::optional<std::string>
saveFile(const std::string &title, const std::string &filter)
{
#ifdef DB_LINUX
    std::string file_filter = filter;
    if (filter.empty())
        file_filter = "All files | *";

    std::string cmd = fmt::format(
            "zenity --file-selection --save --confirm-overwrite --title=\"{}\" --file-filter='{}'",
            title, file_filter);

    static constexpr int size = 2048;
    char buf[size];
    FILE *f = popen(cmd.c_str(), "r");
    fgets(buf, size, f);
    int ret = pclose(f);

    if (ret < 0)
        return std::nullopt;

    std::string output(buf);
    std::string_view view(output);
    auto index = view.find_first_of('\n');
    if (index != std::string_view::npos)
        output.resize(index);

    return output;
#else
#error "Platform does not support save file..."
#endif
}
} // namespace dogb::UT