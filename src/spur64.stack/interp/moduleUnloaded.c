/* Extracted from interp.c:57943 (function moduleUnloaded). */

EXPORT(void)
moduleUnloaded(char *aModuleName)
{
	if (!(strcmp(aModuleName, "SurfacePlugin"))) {
		showSurfaceFn = 0;
	}
}