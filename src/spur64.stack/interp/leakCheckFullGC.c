/* Extracted from interp.c:38107 (function leakCheckFullGC). */

int
leakCheckFullGC(void)
{
	return ((checkForLeaks & GCModeFull) != 0);
}