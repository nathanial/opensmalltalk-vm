/* Extracted from interp.c:57896 (function methodUsesAlternateBytecodeSet). */

int
methodUsesAlternateBytecodeSet(sqInt aMethodObj)
{
    sqInt methodHeader;

	/* begin methodHeaderOf: */
	assert(isCompiledMethod(aMethodObj));
	methodHeader = longAt((void *)((aMethodObj + BaseHeaderSize) + ((((usqInt)(HeaderIndex) << (shiftForWord()))))));

	/* begin headerIndicatesAlternateBytecodeSet: */
	return (((sqLong) methodHeader)) < 0;
}