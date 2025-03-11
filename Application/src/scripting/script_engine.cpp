#include <mono/metadata/assembly.h>
#include <mono/jit/jit.h>
#include "log.hpp"
#include "file.hpp"
#include "script_engine.hpp"


/// <summary>
/// Credit: https://www.youtube.com/watch?v=EKnJOwGhPj4
/// </summary>
namespace Tank
{
	struct ScriptEngineData
	{
		MonoDomain *rootDomain = nullptr;
		MonoDomain *appDomain = nullptr;

		MonoAssembly *coreAssembly = nullptr;
	};
	static ScriptEngineData *s_data = nullptr;


	MonoAssembly *loadCSharpAssembly(const std::filesystem::path &fp)
	{
		int fileSize = 0;
		char *fileData = File::readBytes(fp, &fileSize);

		MonoImageOpenStatus status;
		MonoImage *image = mono_image_open_from_data_full(fileData, fileSize, true, &status, false);

		if (status != MONO_IMAGE_OK)
		{
			TE_CORE_CRITICAL(mono_image_strerror(status));
			return nullptr;
		}

		MonoAssembly *assembly = mono_assembly_load_from_full(image, fp.string().c_str(), &status, false);
		mono_image_close(image);

		delete[] fileData;

		return assembly;
	}


	void printAssemblyTypes(MonoAssembly *assembly)
	{
		MonoImage *image = mono_assembly_get_image(assembly);
		const MonoTableInfo *typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		for (int i = 0; i < numTypes; i++)
		{
			unsigned cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char *nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char *name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

			TE_CORE_INFO(std::string{} + nameSpace + "." + name);
		}
	}


	void ScriptEngine::init()
	{
		s_data = new ScriptEngineData();
		initMono();
	}


	void ScriptEngine::initMono()
	{
		const std::string path = std::string{} + APP_DIRECTORY + "mono/lib";
		mono_set_assemblies_path(path.c_str());

		MonoDomain *rootDomain = mono_jit_init("TankJITRuntime");
		
		if (!rootDomain)
		{
			TE_CORE_CRITICAL("Failed to init script engine.");
			return;
		}

		s_data->rootDomain = rootDomain;

		char appDomainName[] = "TankScriptRuntime";
		s_data->appDomain = mono_domain_create_appdomain(appDomainName, nullptr);

		mono_domain_set(s_data->appDomain, true);

		s_data->coreAssembly = loadCSharpAssembly(std::string{} + SCRIPTCORE_DIRECTORY + "Tank-ScriptCore.dll");
		printAssemblyTypes(s_data->coreAssembly);
	}


	void ScriptEngine::shutdown()
	{
		delete s_data;
		shutdownMono();
	}


	void ScriptEngine::shutdownMono()
	{

	}
}