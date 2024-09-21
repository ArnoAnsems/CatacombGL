#include <string.h>
#include "SDL_render.h"

#include "be_cross.h"
#include "be_st.h"

void BEL_ST_ParseSetting_DisplayNum(int *displayNum, const char *buffer)
{
	if (sscanf(buffer, "%d", displayNum) == 1)
		if ((*displayNum < 0) || (*displayNum >= SDL_GetNumVideoDisplays()))
			*displayNum = 0;
}

void BEL_ST_ParseSetting_SDLRendererDriver(int *driver, const char *buffer)
{
	SDL_RendererInfo info;
	for (int i = 0; i < SDL_GetNumRenderDrivers(); ++i)
	{
		SDL_GetRenderDriverInfo(i, &info);
		if (!strcmp(info.name, buffer))
		{
			*driver = i;
			break;
		}
	}
}

void BEL_ST_SaveSDLRendererDriverToConfig(FILE *fp, const char *key, int driver)
{
	if (driver < 0)
		fprintf(fp, "%s=auto\n", key);
	else
	{
		SDL_RendererInfo info;
		SDL_GetRenderDriverInfo(g_refKeenCfg.sdlRendererDriver, &info); // g_refKeenCfg.sdlRendererDriver should be a valid renderer driver index here
		fprintf(fp, "%s=%s\n", key, info.name);
	}
}
