/* Extracted from interp.c:40739 (function methodHeaderOf). */

static NoDbgRegParms sqInt
methodHeaderOf(sqInt methodObj)
{
	assert(isCompiledMethod(methodObj));
	return longAt((void *)((methodObj + BaseHeaderSize) + ((((usqInt)(HeaderIndex) << (shiftForWord()))))));
}