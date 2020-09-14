#include "VST.h"
#include <filesystem>
#include <iostream>

VSTDatabase::VSTDatabase()
{
}

VSTDatabase::~VSTDatabase()
{
}

bool VSTDatabase::AddDirectory(const std::filesystem::path &pathDirectory)
{
    if (!std::filesystem::is_directory(pathDirectory))
        return false;
    m_vDirectories.push_back(pathDirectory);
    return true;
}

void VSTDatabase::ClearDirectories()
{
    m_vDirectories.clear();
}

bool VSTDatabase::ScanDirectories()
{
    for (auto const &directory : m_vDirectories)
    {
        for (auto const &entry : std::filesystem::recursive_directory_iterator(directory))
        {
            if (!entry.is_regular_file())
                continue;

            std::shared_ptr<VSTPlugin> plugin = std::make_shared<VSTPlugin>();
            if (!plugin->Initialize(entry.path()))
                continue; // Not a valid VST plugin

            plugin->Unload();
            m_vVSTPlugins.push_back(plugin);
        }
    }

    return true;
}

const std::vector<std::shared_ptr<VSTPlugin>> &VSTDatabase::GetPlugins() const
{
    return m_vVSTPlugins;
}

VSTPlugin::VSTPlugin()
    : m_pfnFstMain(nullptr), m_pEffect(nullptr)
{
}

intptr_t dispatch_v(VST24::AEffect *effect, VST24::PluginOpCode_t opcode, int index, intptr_t ivalue, void *ptr, float fvalue)
{
    if (!effect)
        return 0xDEAD;

    const std::string strOpCode = OpCodeToString(opcode);
    printf("Host2Plugin(%s, %d, %lu, %p, %f)\n", strOpCode.c_str(), index, ivalue, ptr, fvalue);

    intptr_t result = effect->dispatcher(effect, static_cast<int32_t>(opcode), index, ivalue, ptr, fvalue);
    printf("Host2Plugin: returned %lu (0x%lX)\n", result, result);
    fflush(stdout);

    return result;
}

VSTPlugin::~VSTPlugin()
{
    Unload();
}

static float db2slider(float f)
{
    return f;
}

static intptr_t dispatcher(VST24::AEffect *effect, int opcode, int index, intptr_t value, void *ptr, float opt)
{
    VST24::HostOpCode_t hostOpCode = static_cast<VST24::HostOpCode_t>(opcode);
    const std::string strOpCodeName = VST24::OpCodeToString(hostOpCode);

    printf("Plugin2Host(%s, %d, %d, %p, %f)\n",
        strOpCodeName.c_str(),
        index,
        (int)value,
        ptr,
        opt
    );

    if (ptr)
    {
        char *str = (char *)ptr;
        if (*str) {
            printf("\t'ptr: %.*s'\n", 512, str);
        }
        else
            printf("\tptr: <nil>\n");
    }


    switch (hostOpCode)
    {
    case VST24::HostOpCode::CurrentId:
        return 0;// xDEAD;
    case VST24::HostOpCode::Version:
        return 2400;
    case VST24::HostOpCode::Automate:
        printf("Automate parameter[%d] to %f\n", index, opt);
        break;
    case VST24::HostOpCode::GetProductString:
        for (size_t i = 0; i < VST24::kMaxProductStrLen; i++) {
            ((char *)ptr)[i] = 64 + i % 60;
        }
        strncpy((char *)ptr, "FstProduct?", VST24::kMaxProductStrLen);
        return 1;
        break;
    case VST24::HostOpCode::GetVendorString:
        strncpy((char *)ptr, "FstVendor?", VST24::kMaxVendorStrLen);
        return 1;
    case VST24::HostOpCode::GetVendorVersion:
        return 1000;
    case VST24::HostOpCode::UpdateDisplay:
        return 0;
    case VST24::HostOpCode::CanDo:
        return 1;
    default:
        printf("\tUnhandled opcode \"%d\"\n", opcode);
        break;
    }
    return 0;
}

bool VSTPlugin::Initialize(const std::filesystem::path &path)
{
    bool bRes(true);

    bRes = bRes && m_plugin.SetPath(path);
    bRes = bRes && m_plugin.Load();
    bRes = bRes &&m_plugin.GetSymbol("VSTPluginMain", reinterpret_cast<void **>(&m_pfnFstMain)) ||
        m_plugin.GetSymbol("main", reinterpret_cast<void **>(&m_pfnFstMain));

    if (bRes)
    {
        m_pEffect = (*m_pfnFstMain)(&dispatcher);
        bRes = bRes && m_pEffect != nullptr;
        bRes = bRes && m_pEffect->nMagic == VST24::kVstMagic;
        if (bRes) std::cout << "Flags: " << VST24::PluginFlagsToString(m_pEffect->nFlags) << std::endl;
        bRes = bRes && dispatch_v(m_pEffect, VST24::PluginOpCode::Initialize, 0, 0, 0, 0.0f) == 0;

        char pProductName[VST24::kMaxProductStrLen + 1] = { 0 };
        bRes = bRes && dispatch_v(m_pEffect, VST24::PluginOpCode::GetProductName, 0, 0, pProductName, 0);
        m_strProductName = pProductName;

        char pEffectName[VST24::kMaxEffectNameStrLen + 1] = { 0 };
        bRes = bRes && dispatch_v(m_pEffect, VST24::PluginOpCode::GetEffectName, 0, 0, pEffectName, 0);
        m_strEffectName = pEffectName;

        char pVendorName[VST24::kMaxVendorStrLen + 1] = { 0 };
        bRes = bRes && dispatch_v(m_pEffect, VST24::PluginOpCode::GetVendorName, 0, 0, pVendorName, 0);
        m_strVendorName = pVendorName;
    }

    m_path = path;

    return bRes;
}

bool VSTPlugin::Unload()
{
    if (!m_pfnFstMain || !m_pEffect)
        return false;

    dispatch_v(m_pEffect, VST24::PluginOpCode::Shutdown, 0, 0, 000, 0.000000);
    m_plugin.Unload();
    m_pfnFstMain = nullptr;
    m_pEffect = nullptr;

    return true;
}

std::filesystem::path VSTPlugin::GetPath() const
{
    return m_path;
}

std::string VSTPlugin::GetProductName() const
{
    return m_strProductName;
}

std::string VSTPlugin::GetEffectName() const
{
    return m_strEffectName;
}

std::string VSTPlugin::GetVendorName() const
{
    return m_strVendorName;
}

int32_t VSTPlugin::GetVersion() const
{
    return m_nVersion;
}
