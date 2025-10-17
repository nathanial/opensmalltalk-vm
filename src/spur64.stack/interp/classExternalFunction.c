/* Extracted from interp.c:33142 (function classExternalFunction). */

	/* SpurMemoryManager>>#classExternalFunction */

sqInt
classExternalFunction(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(ClassExternalFunction) << (shiftForWord()))))));
}