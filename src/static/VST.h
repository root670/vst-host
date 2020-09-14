#pragma once

#include "SharedLibrary.h"
#include "vst24.h"
#include <vector>

class VSTPlugin
{
public:
	VSTPlugin();
	~VSTPlugin();

	/// @brief Try to load the shared library at `path` as a VST plugin.
	/// @param path Path to VST plugin.
	/// @return true if `path` points to a valid VST plugin and it was successfully loaded.
	bool Initialize(const std::filesystem::path &path);

	///Get path to VST plugin.
	std::filesystem::path GetPath()const;

	/// Get product name.
	std::string GetProductName()const;

	/// Get effect name.
	std::string GetEffectName()const;

	/// Get vendor name string.
	std::string GetVendorName()const;

	/// Get version.
	int32_t GetVersion()const;

protected:
	std::filesystem::path	m_path;
	std::string				m_strEffectName;
	std::string				m_strProductName;
	std::string				m_strVendorName;
	int32_t					m_nVersion;
	SharedLibrary			m_plugin;
	VST24::pfnMain*			m_pfnFstMain;
	VST24::AEffect*			m_pEffect;
};

class VSTDatabase
{
public:
	VSTDatabase();
	~VSTDatabase();

	/// @brief Add path to directory list.
	/// @param pathDirectory 
	/// @return true if pathDirectory is a directory. Otherwise, false.
	bool AddDirectory(const std::filesystem::path &pathDirectory);

	/// @brief Clear directory list.
	void ClearDirectories();

	/// @brief Scan all directories for VST plugins.
	/// @return true if scan was successful. Otherwise, false.
	bool ScanDirectories();

protected:
	std::vector<std::filesystem::path>		m_vDirectories;
	std::vector<std::unique_ptr<VSTPlugin>>	m_vVSTPlugins;
};
