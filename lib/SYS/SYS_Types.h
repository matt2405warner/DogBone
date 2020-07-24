//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_SYS_TYPES_H
#define DOGBONE_SYS_TYPES_H

#include "API.h"

#include <cstdint>
#include <string>

namespace dogb
{
using I8 = int8_t;
using I16 = int16_t;
using I32 = int32_t;
using I64 = int64_t;

using U8 = uint8_t;
using U16 = uint16_t;
using U32 = uint32_t;
using U64 = uint64_t;

using C8 = char;
using C16 = char16_t;
using C32 = char32_t;

using String = std::string;

using size_t = std::size_t;

using F32 = float;
using F64 = double;
} // namespace dogb

#endif // DOGBONE_SYS_TYPES_H
