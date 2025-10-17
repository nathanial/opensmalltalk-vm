/* Extracted from interp.c:60268 (function printMethodDictionaryOf). */

/*	useful for VM debugging */

	/* StackInterpreter>>#printMethodDictionaryOf: */

void
printMethodDictionaryOf(sqInt behavior)
{
	printMethodDictionary(longAt((void *)((behavior + BaseHeaderSize) + ((((usqInt)(MethodDictionaryIndex) << (shiftForWord())))))));
}