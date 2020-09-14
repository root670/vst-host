#pragma once
/*
 * Structures, enumerations, constants, etc. used by the VST 2.4 API. Based on vst-rs.
 */

#include <cstdint>
#include <cstring>
#include <string>

#define DEPRECATED(name)    _##name

namespace VST24
{
    /// @brief  Constants
    const unsigned int kMaxPresetNameLen = 24;
    const unsigned int kMaxParamStrLen = 32;
    const unsigned int kMaxLabel = 64;
    const unsigned int kMaxShortLabel = 8;
    const unsigned int kMaxProductStrLen = 64;
    const unsigned int kMaxVendorStrLen = 64;
    const unsigned int kMaxEffectNameStrLen = 32;
    const uint32_t kVstMagic = 0x56737450; // "VstP"

    typedef enum class PluginCategory
    {
        /// Unknown / not implemented
        Unknown,
        /// Any effect
        Effect,
        /// VST instrument
        Synth,
        /// Scope, tuner, spectrum analyser, etc.
        Analysis,
        /// Dynamics, etc.
        Mastering,
        /// Panners, etc.
        Spacializer,
        /// Delays and Reverbs
        RoomFx,
        /// Dedicated surround processor.
        SurroundFx,
        /// Denoiser, etc.
        Restoration,
        /// Offline processing.
        OfflineProcess,
        /// Contains other plugins.
        Shell,
        /// Tone generator, etc.
        Generator,
    } PluginCategory_t;

    typedef enum class PluginFlags
    {
        /// Plugin has editor.
        HasEditor = 1 << 0,
        /// Plugin can process 32-bit audio. Mandatory in VST 2.4.
        CanReplacing = 1 << 4,
        /// Plugin preset data is handled in formatless chunks.
        ProgramChunks = 1 << 5,
        /// Plugin is a synth.
        IsSynth = 1 << 8,
        /// Plugin does not produce sound when all input is silence.
        NoSoundInStop = 1 << 9,
        /// Supports 64-bit audio processing.
        CanDoubleReplacing = 1 << 12,
    } PluginFlags_t;

    std::string PluginFlagsToString(int32_t nFlags);

    /// @brief Operations requested by a host and implemented by a plugin.
    typedef enum class PluginOpCode
    {
        /// Called when plugin is initialized.
        Initialize,
        /// Called when plugin is being shut down.
        Shutdown,

        /// [value]: preset number to change to.
        ChangePreset,
        /// [return]: current preset number.
        GetCurrentPresetNum,
        /// [ptr]: char array with new preset name, limited to `kMaxPresetNameLen`.
        SetCurrentPresetName,
        /// [ptr]: char buffer for current preset name, limited to `kMaxPresetNameLen`.
        GetCurrentPresetName,

        /// [index]: parameter
        /// [ptr]: char buffer, limited to `kMaxParamStrLen` (e.g. "db", "ms", etc)
        GetParameterLabel,
        /// [index]: paramter
        /// [ptr]: char buffer, limited to `kMaxParamStrLen` (e.g. "0.5", "ROOM", etc).
        GetParameterDisplay,
        /// [index]: parameter
        /// [ptr]: char buffer, limited to `kMaxParamStrLen` (e.g. "Release", "Gain")
        GetParameterName,

        /// Deprecated.
        DEPRECATED(GetVu),

        /// [opt]: new sample rate.
        SetSampleRate,
        /// [value]: new maximum block size.
        SetBlockSize,
        /// [value]: 1 when plugin enabled, 0 when disabled.
        StateChanged,

        /// [ptr]: Rect** receiving pointer to editor size.
        EditorGetRect,
        /// [ptr]: system dependent window pointer, eg HWND on Windows.
        EditorOpen,
        /// Close editor. No arguments.
        EditorClose,

        /// Deprecated.
        DEPRECATED(EditorDraw),
        /// Deprecated.
        DEPRECATED(EditorMouse),
        /// Deprecated.
        DEPRECATED(EditorKey),

        /// Idle call from host.
        EditorIdle,

        /// Deprecated.
        DEPRECATED(EditorTop),
        /// Deprecated.
        DEPRECATED(EditorSleep),
        /// Deprecated.
        DEPRECATED(EditorIdentify),

        /// [ptr]: pointer for chunk data address (void**).
        /// [index]: 0 for bank, 1 for program
        GetData,
        /// [ptr]: data (void*)
        /// [value]: data size in bytes
        /// [index]: 0 for bank, 1 for program
        SetData,

        /// [ptr]: VstEvents* TODO: Events
        ProcessEvents,
        /// [index]: param index
        /// [return]: 1=true, 0=false
        CanBeAutomated,
        ///  [index]: param index
        ///  [ptr]: parameter string
        ///  [return]: true for success
        StringToParameter,

        /// Deprecated.
        DEPRECATED(GetNumCategories),

        /// [index]: program name
        /// [ptr]: char buffer for name, limited to `kMaxPresetNameLen`
        /// [return]: true for success
        GetPresetName,

        /// Deprecated.
        DEPRECATED(CopyPreset),
        /// Deprecated.
        DEPRECATED(ConnectIn),
        /// Deprecated.
        DEPRECATED(ConnectOut),

        /// [index]: input index
        /// [ptr]: `VstPinProperties`
        /// [return]: 1 if supported
        GetInputInfo,
        /// [index]: output index
        /// [ptr]: `VstPinProperties`
        /// [return]: 1 if supported
        GetOutputInfo,
        /// [return]: `PluginCategory` category.
        GetCategory,

        /// Deprecated.
        DEPRECATED(GetCurrentPosition),
        /// Deprecated.
        DEPRECATED(GetDestinationBuffer),

        /// [ptr]: `VstAudioFile` array
        /// [value]: count
        /// [index]: start flag
        OfflineNotify,
        /// [ptr]: `VstOfflineTask` array
        /// [value]: count
        OfflinePrepare,
        /// [ptr]: `VstOfflineTask` array
        /// [value]: count
        OfflineRun,

        /// [ptr]: `VstVariableIo`
        /// [use]: used for variable I/O processing (offline e.g. timestretching)
        ProcessVarIo,
        /// TODO: implement
        /// [value]: input `*mut VstSpeakerArrangement`.
        /// [ptr]: output `*mut VstSpeakerArrangement`.
        SetSpeakerArrangement,

        /// Deprecated.
        DEPRECATED(SetBlocksizeAndSampleRate),

        /// Soft bypass (automatable).
        /// [value]: 1 = bypass, 0 = nobypass.
        SoftBypass,
        // [ptr]: buffer for effect name, limited to `kVstMaxEffectNameLen`
        GetEffectName,

        /// Deprecated.
        DEPRECATED(GetErrorText),

        /// [ptr]: buffer for vendor name, limited to `kMaxVendorStrLen`.
        GetVendorName,
        /// [ptr]: buffer for product name, limited to `kMaxProductStrLen`.
        GetProductName,
        /// [return]: vendor specific version.
        GetVendorVersion,
        /// no definition, vendor specific.
        VendorSpecific,
        /// [ptr]: "Can do" string.
        /// [return]: 1 = yes, 0 = maybe, -1 = no.
        CanDo,
        /// [return]: tail size (e.g. reverb time). 0 is defualt, 1 means no tail.
        GetTailSize,

        /// Deprecated.
        DEPRECATED(Idle),
        /// Deprecated.
        DEPRECATED(GetIcon),
        /// Deprecated.
        DEPRECATED(SetVewPosition),

        /// [index]: param index
        /// [ptr]: `*mut VstParamInfo` //TODO: Implement
        /// [return]: 1 if supported
        GetParamInfo,

        /// Deprecated.
        DEPRECATED(KeysRequired),

        /// [return]: 2400 for vst 2.4.
        GetApiVersion,

        /// [index]: ASCII char.
        /// [value]: `Key` keycode.
        /// [opt]: `flags::modifier_key` bitmask.
        /// [return]: 1 if used.
        EditorKeyDown,
        /// [index]: ASCII char.
        /// [value]: `Key` keycode.
        /// [opt]: `flags::modifier_key` bitmask.
        /// [return]: 1 if used.
        EditorKeyUp,
        /// [value]: 0 = circular, 1 = circular relative, 2 = linear.
        EditorSetKnobMode,

        /// [index]: MIDI channel.
        /// [ptr]: `*mut MidiProgramName`. //TODO: Implement
        /// [return]: number of used programs, 0 = unsupported.
        GetMidiProgramName,
        /// [index]: MIDI channel.
        /// [ptr]: `*mut MidiProgramName`. //TODO: Implement
        /// [return]: index of current program.
        GetCurrentMidiProgram,
        /// [index]: MIDI channel.
        /// [ptr]: `*mut MidiProgramCategory`. //TODO: Implement
        /// [return]: number of used categories.
        GetMidiProgramCategory,
        /// [index]: MIDI channel.
        /// [return]: 1 if `MidiProgramName` or `MidiKeyName` has changed. //TODO: Implement
        HasMidiProgramsChanged,
        /// [index]: MIDI channel.
        /// [ptr]: `*mut MidiKeyName`. //TODO: Implement
        /// [return]: 1 = supported 0 = not.
        GetMidiKeyName,

        /// Called before a preset is loaded.
        BeginSetPreset,
        /// Called after a preset is loaded.
        EndSetPreset,

        /// [value]: inputs `*mut VstSpeakerArrangement` //TODO: Implement
        /// [ptr]: Outputs `*mut VstSpeakerArrangement`
        GetSpeakerArrangement,
        /// [ptr]: buffer for plugin name, limited to `kMaxProductStrLen`.
        /// [return]: next plugin's uniqueID.
        ShellGetNextPlugin,

        /// No args. Called once before start of process call. This indicates that the process call
        /// will be interrupted (e.g. Host reconfiguration or bypass when plugin doesn't support
        /// SoftBypass)
        StartProcess,
        /// No arguments. Called after stop of process call.
        StopProcess,
        /// [value]: number of samples to process. Called in offline mode before process.
        SetTotalSampleToProcess,
        /// [value]: pan law `PanLaw`. //TODO: Implement
        /// [opt]: gain.
        SetPanLaw,

        /// [ptr]: `*mut VstPatchChunkInfo`. //TODO: Implement
        /// [return]: -1 = bank cant be loaded, 1 = can be loaded, 0 = unsupported.
        BeginLoadBank,
        /// [ptr]: `*mut VstPatchChunkInfo`. //TODO: Implement
        /// [return]: -1 = bank cant be loaded, 1 = can be loaded, 0 = unsupported.
        BeginLoadPreset,

        /// [value]: 0 if 32 bit, anything else if 64 bit.
        SetPrecision,

        /// [return]: number of used MIDI Inputs (1-15).
        GetNumMidiInputs,
        /// [return]: number of used MIDI Outputs (1-15).
        GetNumMidiOutputs,
    } PluginOpCode_t;

    /// @brief Operations requested by a plugin and implemented by a host.
    typedef enum class HostOpCode
    {
        /// [index]: parameter index
        /// [opt]: parameter value
        Automate = 0,
        /// [return]: host vst version (e.g. 2400 for VST 2.4)
        Version,
        /// [return]: current plugin ID (useful for shell plugins to figure out which plugin to load in
        ///           `VSTPluginMain()`).
        CurrentId,
        /// No arguments. Give idle time to Host application, e.g. if plug-in editor is doing mouse
        /// tracking in a modal loop.
        Idle,
        /// Deprecated.
        DEPRECATED(PinConnected) = 4,

        /// Deprecated.
        DEPRECATED(WantMidi) = 6, // Not a typo
        /// [value]: request mask. see `VstTimeInfoFlags`
        /// [return]: `VstTimeInfo` pointer or null if not supported.
        GetTime,
        /// Inform host that the plugin has MIDI events ready to be processed. Should be called at the
        /// end of `Plugin::process`.
        /// [ptr]: `VstEvents*` the events to be processed.
        /// [return]: 1 if supported and processed OK.
        ProcessEvents,
        /// Deprecated.
        DEPRECATED(SetTime),
        /// Deprecated.
        DEPRECATED(TempoAt),
        /// Deprecated.
        DEPRECATED(GetNumAutomatableParameters),
        /// Deprecated.
        DEPRECATED(GetParameterQuantization),

        /// Notifies the host that the input/output setup has changed. This can allow the host to check
        /// numInputs/numOutputs or call `getSpeakerArrangement()`.
        /// [return]: 1 if supported.
        IOChanged,

        /// Deprecated.
        DEPRECATED(NeedIdle),

        /// Request the host to resize the plugin window.
        /// [index]: new width.
        /// [value]: new height.
        SizeWindow,
        /// [return]: the current sample rate.
        GetSampleRate,
        /// [return]: the current block size.
        GetBlockSize,
        /// [return]: the input latency in samples.
        GetInputLatency,
        /// [return]: the output latency in samples.
        GetOutputLatency,

        /// Deprecated.
        DEPRECATED(GetPreviousPlug),
        /// Deprecated.
        DEPRECATED(GetNextPlug),
        /// Deprecated.
        DEPRECATED(WillReplaceOrAccumulate),

        /// [return]: the current process level, see `VstProcessLevels`
        GetCurrentProcessLevel,
        /// [return]: the current automation state, see `VstAutomationStates`
        GetAutomationState,

        /// The plugin is ready to begin offline processing.
        /// [index]: number of new audio files.
        /// [value]: number of audio files.
        /// [ptr]: `AudioFile*` the host audio files. Flags can be updated from plugin.
        OfflineStart,
        /// Called by the plugin to read data.
        /// [index]: (bool)
        ///    VST offline processing allows a plugin to overwrite existing files. If this value is
        ///    true then the host will read the original file's samples, but if it is false it will
        ///    read the samples which the plugin has written via `OfflineWrite`
        /// [value]: see `OfflineOption`
        /// [ptr]: `OfflineTask*` describing the task.
        /// [return]: 1 on success
        OfflineRead,
        /// Called by the plugin to write data.
        /// [value]: see `OfflineOption`
        /// [ptr]: `OfflineTask*` describing the task.
        OfflineWrite,
        /// Unknown. Used in offline processing.
        OfflineGetCurrentPass,
        /// Unknown. Used in offline processing.
        OfflineGetCurrentMetaPass,

        /// Deprecated.
        DEPRECATED(SetOutputSampleRate),
        /// Deprecated.
        DEPRECATED(GetOutputSpeakerArrangement),

        /// Get the vendor string.
        /// [ptr]: `char*` for vendor string, limited to `kMaxVendorStrLen`.
        GetVendorString,
        /// Get the product string.
        /// [ptr]: `char*` for vendor string, limited to `kMaxProductStrLen`.
        GetProductString,
        /// [return]: vendor-specific version
        GetVendorVersion,
        /// Vendor specific handling.
        VendorSpecific,

        /// Deprecated.
        DEPRECATED(SetIcon),

        /// Check if the host supports a feature.
        /// [ptr]: `char*` can do string
        /// [return]: 1 if supported
        CanDo,
        /// Get the language of the host.
        /// [return]: `VstHostLanguage`
        GetLanguage,

        /// Deprecated.
        DEPRECATED(OpenWindow),
        /// Deprecated.
        DEPRECATED(CloseWindow),

        /// Get the current directory.
        /// [return]: `FSSpec` on OS X, `char*` otherwise
        GetDirectory,
        /// Tell the host that the plugin's parameters have changed, refresh the UI.
        ///
        /// No arguments.
        UpdateDisplay,
        /// Tell the host that if needed, it should record automation data for a control.
        ///
        /// Typically called when the plugin editor begins changing a control.
        ///
        /// [index]: index of the control.
        /// [return]: true on success.
        BeginEdit,
        /// A control is no longer being changed.
        ///
        /// Typically called after the plugin editor is done.
        ///
        /// [index]: index of the control.
        /// [return]: true on success.
        EndEdit,
        /// Open the host file selector.
        /// [ptr]: `VstFileSelect*`
        /// [return]: true on success.
        OpenFileSelector,
        /// Close the host file selector.
        /// [ptr]: `VstFileSelect*`
        /// [return]: true on success.
        CloseFileSelector,

        /// Deprecated.
        DEPRECATED(EditFile),
        /// Deprecated.
        /// [ptr]: char[2048] or sizeof (FSSpec).
        /// [return]: 1 if supported.
        DEPRECATED(GetChunkFile),
        /// Deprecated.
        DEPRECATED(GetInputSpeakerArrangement),
    } HostOpCode_t;

    std::string OpCodeToString(HostOpCode_t nOpCode);

    std::string OpCodeToString(PluginOpCode_t nOpCode);

    /// @brief Data structures
    struct AEffect;
    typedef intptr_t(*pfnDispatcherProc) (AEffect *pEffect, int32_t nOpCode, int32_t nIndex, intptr_t nValue, void *pPtr, float fOpt);
    typedef void (*pfnProcessProc) (AEffect *pEffect, float **ppfInputs, float **ppfOutputs, int32_t nSampleFrames);
    typedef void (*pfnProcessProcDouble) (AEffect *pEffect, double **ppfInputs, double **ppfOutputs, int32_t nSampleFrames);
    typedef void (*pfnSetParameterProc) (AEffect *pEffect, int32_t nIndex, float fParameter);
    typedef float (*pfnGetParameterProc) (AEffect *pEffect, int32_t nIndex);
    typedef AEffect *(pfnMain)(pfnDispatcherProc);

    /// @brief Used by the VST API to pass around plugin information.
    struct AEffect
    {
        AEffect()
            : nMagic(kVstMagic), nReserved1(0), nReserved2(0)
        {
            memset(padding, 0, sizeof(padding));
        }

        /// Magic number. Must be kVstMagic.
        uint32_t nMagic;
        /// Host to plug-in dispatcher.
        pfnDispatcherProc dispatcher;
        /// Accumulating process mode is deprecated in VST 2.4! Use `processReplacing` instead!
        pfnProcessProc DEPRECATED(process);
        /// Set value of automatable parameter. 
        pfnSetParameterProc setParameter;
        /// Get value of automatable parameter.
        pfnGetParameterProc getParameter;

        /// Number of programs (presets).
        int32_t nNumPrograms;
        /// Number of parameters. All programs are assumed to have this many parameters.
        int32_t nNumParams;
        /// Number of audio inputs.
        int32_t nNumInputs;
        /// Number of audio outputs.
        int32_t nNumOutputs;

        /// Bitmask made of values from `PluginFlags_t`.
        int32_t nFlags;

        /// Reserved for host, must be 0.
        int32_t nReserved1;
        /// Reserved for host, must be 0.
        int32_t nReserved2;

        /// For algorithms which need input in the first place (Group delay or latency in samples).
        /// This value should be initially in a resume state.
        int32_t nInitialDelay;

        /// Deprecated
        int32_t DEPRECATED(nRealQualities);
        /// Deprecated
        int32_t DEPRECATED(nOffQualities);
        /// Deprecated
        int32_t DEPRECATED(nIORatio);

        /// Void pointer usable by API to store object data.
        void *pObject;
        /// User-defined pointer.
        void *pUser;

        /// Registered unique identifier (register it at $teinberg 3rd party support Web).
        /// This is used to identify a plug-in during save/load of preset and project.
        int32_t nUniqueID;

        /// Plug-in version (e.g. 1234 for v1.2.3.4).
        int32_t nVersion;

        /// Process audio samples in replacing mode.
        pfnProcessProc processReplacing;
        /// Process double-precision audio samples in replacing mode.
        pfnProcessProcDouble processReplacingDouble;

        char padding[56];
    };

} // namespace VST24
