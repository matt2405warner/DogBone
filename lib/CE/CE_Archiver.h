//
// Created by matt on 2020-08-07.
//

#ifndef DOGBONE_CE_ARCHIVER_H
#define DOGBONE_CE_ARCHIVER_H

#include "CE_API.h"

#include <yaml-cpp/yaml.h>

namespace dogb::CE
{

// Not sure what the end plan will be but alias these types so that its easier
// to modify in the future.
using ArchiveEmitter = YAML::Emitter;
using ArchiveParser = YAML::Parser;
using ArchiveNode = YAML::Node;

} // namespace dogb::CE

#endif // DOGBONE_CE_ARCHIVER_H
