/* Extracted from interp.c:45681 (function splObj). */

/*	Return one of the objects in the specialObjectsArray */

	/* SpurMemoryManager>>#splObj: */

sqInt
splObj(sqInt index)
{
	return longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(index) << (shiftForWord()))))));
}