#include "VST24.h"
#include <vector>

namespace VST24
{
    std::string PluginFlagsToString(int32_t nFlags)
    {
        std::vector<std::string> vFlags;

        if (nFlags & static_cast<int>(PluginFlags::HasEditor))
            vFlags.push_back("HasEditor");
        if (nFlags & static_cast<int>(PluginFlags::CanReplacing))
            vFlags.push_back("CanReplacing");
        if (nFlags & static_cast<int>(PluginFlags::ProgramChunks))
            vFlags.push_back("ProgramChunks");
        if (nFlags & static_cast<int>(PluginFlags::IsSynth))
            vFlags.push_back("IsSynth");
        if (nFlags & static_cast<int>(PluginFlags::NoSoundInStop))
            vFlags.push_back("NoSoundInStop");
        if (nFlags & static_cast<int>(PluginFlags::CanDoubleReplacing))
            vFlags.push_back("CanDoubleReplacing");

        std::string strFlags;
        for (size_t nFlag = 0; nFlag < vFlags.size(); nFlag++)
        {
            if (nFlag > 0)
                strFlags += ", ";
            strFlags += vFlags[nFlag];
        }

        return strFlags;
    }

    std::string OpCodeToString(HostOpCode_t nOpCode)
    {
        #define ENUM_CASE(value)   case HostOpCode_t::##value##: return #value;
        switch (nOpCode)
        {
            ENUM_CASE(Automate)
            ENUM_CASE(Version)
            ENUM_CASE(CurrentId)
            ENUM_CASE(Idle)
            ENUM_CASE(GetTime)
            ENUM_CASE(ProcessEvents)
            ENUM_CASE(IOChanged)
            ENUM_CASE(SizeWindow)
            ENUM_CASE(GetSampleRate)
            ENUM_CASE(GetBlockSize)
            ENUM_CASE(GetInputLatency)
            ENUM_CASE(GetOutputLatency)
            ENUM_CASE(GetCurrentProcessLevel)
            ENUM_CASE(GetAutomationState)
            ENUM_CASE(OfflineStart)
            ENUM_CASE(OfflineRead)
            ENUM_CASE(OfflineWrite)
            ENUM_CASE(OfflineGetCurrentPass)
            ENUM_CASE(OfflineGetCurrentMetaPass)
            ENUM_CASE(GetVendorString)
            ENUM_CASE(GetProductString)
            ENUM_CASE(GetVendorVersion)
            ENUM_CASE(VendorSpecific)
            ENUM_CASE(CanDo)
            ENUM_CASE(GetLanguage)
            ENUM_CASE(GetDirectory)
            ENUM_CASE(UpdateDisplay)
            ENUM_CASE(BeginEdit)
            ENUM_CASE(EndEdit)
            ENUM_CASE(OpenFileSelector)
            ENUM_CASE(CloseFileSelector)
        }
        return "(unknown)";
        #undef ENUM_CASE
    }

    std::string OpCodeToString(PluginOpCode_t nOpCode)
    {
        #define ENUM_CASE(value)   case PluginOpCode_t::##value##: return #value;
        switch (nOpCode)
        {
            ENUM_CASE(Initialize)
            ENUM_CASE(Shutdown)
            ENUM_CASE(ChangePreset)
            ENUM_CASE(GetCurrentPresetNum)
            ENUM_CASE(SetCurrentPresetName)
            ENUM_CASE(GetCurrentPresetName)
            ENUM_CASE(GetParameterLabel)
            ENUM_CASE(GetParameterDisplay)
            ENUM_CASE(GetParameterName)
            ENUM_CASE(SetSampleRate)
            ENUM_CASE(SetBlockSize)
            ENUM_CASE(StateChanged)
            ENUM_CASE(EditorGetRect)
            ENUM_CASE(EditorOpen)
            ENUM_CASE(EditorClose)
            ENUM_CASE(EditorIdle)
            ENUM_CASE(GetData)
            ENUM_CASE(SetData)
            ENUM_CASE(ProcessEvents)
            ENUM_CASE(CanBeAutomated)
            ENUM_CASE(StringToParameter)
            ENUM_CASE(GetPresetName)
            ENUM_CASE(GetInputInfo)
            ENUM_CASE(GetOutputInfo)
            ENUM_CASE(GetCategory)
            ENUM_CASE(OfflineNotify)
            ENUM_CASE(OfflinePrepare)
            ENUM_CASE(OfflineRun)
            ENUM_CASE(ProcessVarIo)
            ENUM_CASE(SetSpeakerArrangement)
            ENUM_CASE(SoftBypass)
            ENUM_CASE(GetEffectName)
            ENUM_CASE(GetVendorName)
            ENUM_CASE(GetProductName)
            ENUM_CASE(GetVendorVersion)
            ENUM_CASE(CanDo)
            ENUM_CASE(GetTailSize)
            ENUM_CASE(GetParamInfo)
            ENUM_CASE(GetApiVersion)
            ENUM_CASE(EditorKeyDown)
            ENUM_CASE(EditorKeyUp)
            ENUM_CASE(EditorSetKnobMode)
            ENUM_CASE(GetMidiProgramName)
            ENUM_CASE(GetCurrentMidiProgram)
            ENUM_CASE(GetMidiProgramCategory)
            ENUM_CASE(HasMidiProgramsChanged)
            ENUM_CASE(GetMidiKeyName)
            ENUM_CASE(BeginSetPreset)
            ENUM_CASE(EndSetPreset)
            ENUM_CASE(GetSpeakerArrangement)
            ENUM_CASE(ShellGetNextPlugin)
            ENUM_CASE(StartProcess)
            ENUM_CASE(StopProcess)
            ENUM_CASE(SetTotalSampleToProcess)
            ENUM_CASE(SetPanLaw)
            ENUM_CASE(BeginLoadBank)
            ENUM_CASE(BeginLoadPreset)
            ENUM_CASE(SetPrecision)
            ENUM_CASE(GetNumMidiInputs)
            ENUM_CASE(GetNumMidiOutputs)
        }
        return "(unknown)";
        #undef ENUM_CASE
    }
} // namespace VST24
