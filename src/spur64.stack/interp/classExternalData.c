/* Extracted from interp.c:33135 (function classExternalData). */

	/* SpurMemoryManager>>#classExternalData */

sqInt
classExternalData(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(ClassExternalData) << (shiftForWord()))))));
}