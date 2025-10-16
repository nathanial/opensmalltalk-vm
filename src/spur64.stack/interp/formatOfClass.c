/* Extracted from interp.c:35710 (function formatOfClass). */

sqInt
formatOfClass(sqInt classPointer)
{
	return ((longAt((void *)((classPointer + BaseHeaderSize) + ((((usqInt)(InstanceSpecificationIndex) << (shiftForWord()))))))) >> 3);
}