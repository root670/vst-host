#pragma once

#include "SharedLibrary.h"
#include <fst.h>
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

protected:
	typedef AEffect *(fnFstMain_t)(AEffectDispatcherProc);
	SharedLibrary	m_plugin;
	fnFstMain_t*	m_pfnFstMain;
	AEffect*		m_pEffect;
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
