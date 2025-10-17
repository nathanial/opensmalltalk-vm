/* Extracted from interp.c:57943 (function moduleUnloaded). */

/*	The module with the given name was just unloaded. 
	Make sure we have no dangling references. */

	/* StackInterpreter>>#moduleUnloaded: */

EXPORT(void)
moduleUnloaded(char *aModuleName)
{
	if (!(strcmp(aModuleName, "SurfacePlugin"))) {
		showSurfaceFn = 0;
	}
}