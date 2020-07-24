//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_API_H
#define DOGBONE_API_H

#ifndef DB_IS_STATIC
#  ifdef DB_PLATFORM_WIN
#    ifdef DB_EXPORT_DLL
#      define DB_DLL_EXPORT __declspec(dllexport)
#    else
#      define DB_DLL_EXPORT __declspec(dllimport)
#    endif // DB_DLL_EXPORT
#  elif __GNUC__ >= 4
#    ifdef DB_EXPORT_DLL
#      define DB_DLL_EXPORT __attribute((visibility("default")))
#    else
#      define DB_DLL_EXPORT
#    endif
#  else
#    define DB_DLL_EXPORT
#  endif
#else
#  define DB_DLL_EXPORT
#endif // DB_DLL_EXPORT

#endif //DOGBONE_API_H
