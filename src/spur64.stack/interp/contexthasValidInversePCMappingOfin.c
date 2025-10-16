/* Extracted from interp.c:51495 (function contexthasValidInversePCMappingOfin). */

static NoDbgRegParms sqInt
contexthasValidInversePCMappingOfin(sqInt aContext, sqInt theIP, char *theFP)
{
    sqInt encodedip;
    sqInt methodHeader;
    sqInt methodObj;
    sqInt pc;

	pc = longAt((void *)((aContext + BaseHeaderSize) + ((((usqInt)(InstructionPointerIndex) << (shiftForWord()))))));

	/* begin contextInstructionPointer:frame: */
	assert(validInstructionPointerinFrame(theIP + 1, theFP));
	encodedip = (((usqInt)(((theIP - (longAt(theFP + FoxMethod))) - BaseHeaderSize) + 2) << 3) | 1);
	return (pc == encodedip)
	 || (((methodObj = longAt((void *)((aContext + BaseHeaderSize) + ((((usqInt)(MethodIndex) << (shiftForWord()))))))),
	/* begin methodHeaderOf: */
		assert(isCompiledMethod(methodObj)),
	(methodHeader = longAt((void *)((methodObj + BaseHeaderSize) + ((((usqInt)(HeaderIndex) << (shiftForWord()))))))),
	(((methodHeader & AlternateHeaderHasPrimFlag) != 0))
		 && ((((encodedip >> 3)) - ((pc >> 3))) == 3 /* sizeOfCallPrimitiveBytecode: */)));
}