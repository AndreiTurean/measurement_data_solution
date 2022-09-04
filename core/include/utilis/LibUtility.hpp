
#pragma once
#ifndef _WIN32
#include <dlfcn.h>
#include <gnu/lib-names.h>
#else
#include <windows.h>
#endif

namespace core
{
    namespace utility
    {
        class LibUtility
        {
        public:
            static void* openLibrary(const char* libName, const char* libSymbols)
            {
                void* handle = nullptr;
#ifndef _WIN32
                handle = dlopen(libName, RTLD_NOW);
#else
                handle = LoadLibrary(libName);
#endif

#ifndef _WIN32
                return dlsym(handle, libSymbols);
#else
                return return (void *)GetProcAddress((HMODULE)handle, libSymbols);
#endif     
            }
            static bool clsoeLibrary(const char* libName);
        };
    }
} // namespace core

