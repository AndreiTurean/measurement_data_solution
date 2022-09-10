
#pragma once
#ifndef _WIN32
#include <dlfcn.h>
#include <gnu/lib-names.h>
#else
#include <windows.h>
#endif

#include <vector>
#include <mutex>

namespace core
{
    namespace utility
    {
        /*!
        *   @brief Lib loader facade
        */
        class LibUtility
        {
            std::vector<void*> handleContainer_; 
        public:
            LibUtility() = default;

            /*!
            *   @brief Class destructor. Close all the library handles left open. Exclude the null handles.
            */
            ~LibUtility()
            {
            }
            /*!
            *   @brief Method receive a library name and will try to load and resolve the symbols.
            *   @param libName path to the library that will be loaded. The path must include the library name.
            *   @param libSymbols library simbol name
            *   @return a pointer to library function
            */
            void* openLibrary(const char* libName, const char* libSymbols)
            {
                void* handle = nullptr;
#ifndef _WIN32
                handle = dlopen(libName, RTLD_NOW);
#else
                handle = LoadLibrary(libName);
#endif
                handleContainer_.push_back(handle);
#ifndef _WIN32
                return dlsym(handle, libSymbols);
#else
                return (void *)GetProcAddress((HMODULE)handle, libSymbols);
#endif     
            }

            /*!
            *   @brief Method used to close the open library handles.
            *   @param libHandle library handle
            */
            static int closeLibrary(void* libHandle)
            {
#ifndef _WIN32
                return dlclose(libHandle);
#else
                return FreeLibrary((HMODULE)libHandle);
#endif
            }
        };
    }
} // namespace core

