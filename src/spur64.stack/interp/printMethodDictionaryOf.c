/* Extracted from interp.c:60268 (function printMethodDictionaryOf). */

void
printMethodDictionaryOf(sqInt behavior)
{
	printMethodDictionary(longAt((void *)((behavior + BaseHeaderSize) + ((((usqInt)(MethodDictionaryIndex) << (shiftForWord())))))));
}