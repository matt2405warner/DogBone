//
// Created by matt on 2020-08-10.
//

#ifndef DOGBONE_IMGUI_FILEBROWSER_H
#define DOGBONE_IMGUI_FILEBROWSER_H

#include "IMGUI_API.h"

#include "IMGUI_Modal.h"
#include "IMGUI_Window.h"

#include <functional>
#include <string>
#include <filesystem>

namespace dogb::IMGUI
{
class DB_IMGUI_API FileBrowser : public Modal
{
public:
    using SelectClb = std::function<void(const std::string &)>;

    void open();
    void close();
    void render() override;

    void setTitle(const std::string &title);

    static FileBrowser *openBrowser(
            const std::string &directory,
            SelectClb select_clb);

    void setFilters(const std::vector<std::string> &filters)
    {
        m_filters = filters;
    }

    // Check if the path matches the filter.
    // NB: This function assumes the path is not a directory.
    [[nodiscard]] bool matchesFilter(const std::filesystem::path& path) const;
private:
    FileBrowser(const std::string &path, SelectClb select_clb);

    std::function<void(const std::string &)> m_selectClb;

    std::string m_currentDirectory;
    std::string m_currentSelection;

    std::vector<std::string> m_filters;
    std::vector<std::string>::size_type m_currentFilter = 0;

    std::string m_title;
    std::string m_openLabel;

    unsigned m_width, m_height;
};

} // namespace dogb::IMGUI

#endif // DOGBONE_IMGUI_FILEBROWSER_H
