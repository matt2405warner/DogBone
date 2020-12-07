//
// Created by matt on 2020-12-07.
//

#ifndef DOGBONE_UT_FILEDIALOG_H
#define DOGBONE_UT_FILEDIALOG_H

#include "UT_API.h"

#include <optional>
#include <string>

namespace dogb::UT
{
DB_UT_API std::optional<std::string> openFile(
        const std::string &title,
        const std::string &filter);
DB_UT_API std::optional<std::string> saveFile(
        const std::string &title,
        const std::string &filter);
} // namespace dogb::UT

#endif // DOGBONE_UT_FILEDIALOG_H
