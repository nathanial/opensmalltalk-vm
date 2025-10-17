/* Extracted from interp.c:33156 (function classExternalStructure). */

	/* SpurMemoryManager>>#classExternalStructure */

sqInt
classExternalStructure(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(ClassExternalStructure) << (shiftForWord()))))));
}