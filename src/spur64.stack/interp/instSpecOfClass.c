/* Extracted from interp.c:37110 (function instSpecOfClass). */

static NoDbgRegParms sqInt
instSpecOfClass(sqInt classPointer)
{
	return (((usqInt)((((longAt((void *)((classPointer + BaseHeaderSize) + ((((usqInt)(InstanceSpecificationIndex) << (shiftForWord()))))))) >> 3)))) >> (fixedFieldsFieldWidth())) & (formatMask());
}