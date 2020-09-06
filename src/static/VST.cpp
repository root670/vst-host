#include "VST.h"
#include "fst_utils.h""
#include <filesystem>

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

			std::unique_ptr<VSTPlugin> plugin = std::make_unique<VSTPlugin>();
			if (!plugin->Initialize(entry.path()))
				continue; // Not a valid VST plugin

			m_vVSTPlugins.emplace_back(std::move(plugin));
		}
	}
	
	return true;
}

VSTPlugin::VSTPlugin()
	: m_pfnFstMain(nullptr)
{
}

static
t_fstPtrInt dispatch_effect(AEffectDispatcherProc dispatchcb,
	AEffect *effect, int opcode, int index, t_fstPtrInt ivalue, void *ptr, float fvalue)
{
	if (!effect)
		return 0xDEAD;
	char opcodestr[256];

	if (!dispatchcb)
		dispatchcb = effect->dispatcher;

	printf("Fst::host2plugin(%s, %d, %lu, %p, %f)\n",
		effCode2string(opcode, opcodestr, 255), index, ivalue, ptr, fvalue);
	print_effPtr(effect, opcode, index, ivalue, ptr, fvalue, 1);

	t_fstPtrInt result = dispatchcb(effect, opcode, index, ivalue, ptr, fvalue);
	printf("Fst::host2plugin: %lu (0x%lX)\n", result, result);
	print_effPtr(effect, opcode, index, ivalue, ptr, fvalue, 2, result);
	fflush(stdout);

	return result;
}

t_fstPtrInt dispatch_v(AEffect *effect, int opcode, int index, t_fstPtrInt ivalue, void *ptr, float fvalue)
{
	return dispatch_effect(effect->dispatcher, effect, opcode, index, ivalue, ptr, fvalue);
}

VSTPlugin::~VSTPlugin()
{
	if (m_pfnFstMain && m_pEffect)
	{
		dispatch_v(m_pEffect, effClose, 0, 0, 000, 0.000000);
	}
}

static float db2slider(float f) {
	return f;
}

static t_fstPtrInt dispatcher(AEffect *effect, int opcode, int index, t_fstPtrInt value, void *ptr, float opt)
{
	char sbuf[256] = { 0 };
	printf("FstHost::dispatcher[%d]", effect ? effect->resvd2 : -1);
	printf("(%s, %d, %d, %p, %f)\n",
		hostCode2string(opcode, sbuf, 255),
		index, (int)value,
		ptr, opt);
	if (ptr)
	{
		char *str = (char *)ptr;
		if (*str) {
			printf("\t'%.*s'\n", 512, str);
		}
		else
			printf("\t<nil>\n");
	}
	switch (opcode)
	{
	case 0xDEADBEEF:
		return (t_fstPtrInt)db2slider;
	case audioMasterCurrentId:
		return 0;// xDEAD;
	case audioMasterVersion:
		printf("MasterVersion\n");
		return 2400;
	case audioMasterAutomate:
		printf("automate parameter[%d] to %f\n", index, opt);
		break;
	case audioMasterGetProductString:
		for (size_t i = 0; i < kVstMaxProductStrLen; i++) {
			((char *)ptr)[i] = 64 + i % 60;
		}
		strncpy((char *)ptr, "FstProduct?", kVstMaxProductStrLen);
		return 1;
		break;
	case audioMasterGetVendorString:
		strncpy((char *)ptr, "FstVendor?", kVstMaxVendorStrLen);
		return 1;
	case 42:
		return 0;
	case audioMasterCanDo:
		printf("MasterCanDo\n");
		return 1;
	default:
		printf("\tdyspatcher(%p, %d, %d, %d, %p, %f);\n", effect, opcode, index, value, ptr, opt);
		do {
			char *str = (char *)ptr;
			if (str && *str) {
				printf("\t'%.*s'\n", 512, str);
			}
		} while (0);
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
		m_plugin.GetSymbol("main", reinterpret_cast<void **>(m_pfnFstMain));
	
	if (bRes)
	{
		m_pEffect = (*m_pfnFstMain)(&dispatcher);
		bRes = bRes && m_pEffect != nullptr;
		bRes = bRes && m_pEffect->magic == kEffectMagic;
		if(bRes) printf("FLAGS: %X\n", m_pEffect->flags);
		bRes = bRes && dispatch_v(m_pEffect, effOpen, 0, 0, 0, 0.0f) == 0;
	}

	return bRes;
}
