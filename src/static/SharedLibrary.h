#pragma once

#include <Windows.h>
#include <string>
#include <filesystem>

class SharedLibrary
{
public:
    SharedLibrary(const std::filesystem::path &path = "");
    ~SharedLibrary();

    /// @brief Set path to shared library.
    /// @param path
    /// @return true if path is a valid shared library path.
    bool SetPath(const std::filesystem::path &path);

    /// @brief Get path to shared library.
    /// @return The path.
    std::filesystem::path GetPath()const;

    /// @brief Load the shared library.
    /// @return true if the shared library was loaded successfully. Otherwise, false.
    bool Load();

    /// @brief Get symbol from loaded shared library.
    /// @param strName Name of symbol to load.
    /// @param ptrFn Pointer to store pointer to symbol.
    /// @return true if the requested symbol was loaded sucessfully and its pointer was assigned to *ptrFn.
    bool GetSymbol(const std::string &strName, void **ptrFn)const;

protected:
    std::filesystem::path	m_path;
    HINSTANCE				m_handle;
};
