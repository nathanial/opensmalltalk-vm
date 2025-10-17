/* Extracted from interp.c:33149 (function classExternalLibrary). */

	/* SpurMemoryManager>>#classExternalLibrary */

sqInt
classExternalLibrary(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(ClassExternalLibrary) << (shiftForWord()))))));
}