/* Extracted from interp.c:33128 (function classExternalAddress). */

	/* SpurMemoryManager>>#classExternalAddress */

sqInt
classExternalAddress(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(ClassExternalAddress) << (shiftForWord()))))));
}