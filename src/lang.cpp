#include "lang.h"
#include "libs/simpleini/SimpleIni.h"

CSimpleIniA ini_lang;


void lang_load(const char* filename)
{
	if (ini_lang.LoadFile(filename) < 0)
	{
		printf("Can't load lang file\n");
		return;
	}
}




const char* lang(const char* section, const char* key)
{
	return ini_lang.GetValue(section, key, "");
}