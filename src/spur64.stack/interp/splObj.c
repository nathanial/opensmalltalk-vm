/* Extracted from interp.c:45681 (function splObj). */

/*	Return one of the objects in the specialObjectsArray */

	/* SpurMemoryManager>>#splObj: */

sqInt
splObj(sqInt index)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(index) << (shiftForWord()))))));
}