#include <string.h>
#include <SDL3/SDL_render.h>

#include "be_cross.h"
#include "be_st.h"

void BEL_ST_ParseSetting_SDLRendererDriver(int *driver, const char *buffer)
{
	for (int i = 0; i < SDL_GetNumRenderDrivers(); ++i)
		if (!strcmp(SDL_GetRenderDriver(i), buffer))
		{
			*driver = i;
			break;
		}
}

void BEL_ST_SaveSDLRendererDriverToConfig(FILE *fp, const char *key, int driver)
{
	if (driver < 0)
		fprintf(fp, "%s=auto\n", key);
	else
	{
		// g_refKeenCfg.sdlRendererDriver should be
		// a valid renderer driver index here
		const char *name = SDL_GetRenderDriver(g_refKeenCfg.sdlRendererDriver);
		fprintf(fp, "%s=%s\n", key, name);
	}
}
