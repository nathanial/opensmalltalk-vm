/* Extracted from interp.c:51594 (function couldBeProcess). */

static NoDbgRegParms NeverInline sqInt
couldBeProcess(sqInt oop)
{
	return (addressCouldBeObj(oop))
	 && ((((byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask())) <= 5 /* lastPointerFormat */)
	 && ((!(((longAt((void *)(oop))) & (classIndexMask())) == ClassMethodContextCompactIndex))
	 && (((lengthOf(oop)) > MyListIndex)
	 && (isContext(longAt((void *)((oop + BaseHeaderSize) + ((((usqInt)(SuspendedContextIndex) << (shiftForWord())))))))))));
}