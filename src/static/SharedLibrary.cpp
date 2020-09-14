#include "SharedLibrary.h"
#include <assert.h>

SharedLibrary::SharedLibrary(const std::filesystem::path &path)
    : m_path(path), m_handle(nullptr)
{}

SharedLibrary::~SharedLibrary()
{
    if (m_handle)
        FreeLibrary(m_handle);
}

bool SharedLibrary::SetPath(const std::filesystem::path &path)
{
    if (path.extension() != ".dll")
        return false;
    m_path = path;
    return true;
}

std::filesystem::path SharedLibrary::GetPath() const
{
    return m_path;
}

bool SharedLibrary::Load()
{
    if (m_handle)
        return true; // Already loaded

    m_handle = LoadLibraryW(m_path.c_str());

    return m_handle != nullptr;
}

bool SharedLibrary::GetSymbol(const std::string &strName, void **ptrFn) const
{
    assert(ptrFn);

    FARPROC pAddress = GetProcAddress(m_handle, strName.c_str());

    if (!pAddress)
        return false;

    *ptrFn = static_cast<void *>(pAddress);
    return true;
}
