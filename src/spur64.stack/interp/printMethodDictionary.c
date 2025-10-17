/* Extracted from interp.c:60278 (function printMethodDictionary). */

/*	useful for VM debugging */

	/* StackInterpreter>>#printMethodDictionary: */

void
printMethodDictionary(sqInt dictionary)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt index;
    sqInt meth;
    sqInt methodArray;
    sqInt selector;
    sqInt toDoLimit;

	methodArray = longAt((void *)((dictionary + BaseHeaderSize) + ((((usqInt)(MethodArrayIndex) << (shiftForWord()))))));
	toDoLimit = (numSlotsOf(dictionary)) - 1;
	for (index = SelectorStart; index <= toDoLimit; index += 1) {
		selector = longAt((void *)((dictionary + BaseHeaderSize) + ((((usqInt)(index) << (shiftForWord()))))));
		if (selector != GIV(nilObj)) {
			meth = longAt((void *)((methodArray + BaseHeaderSize) + ((((usqInt)((index - SelectorStart)) << (shiftForWord()))))));
			printOopShortInner(selector);
			print(" => ");
			printOopShortInner(meth);
			print(" (");
			printHex(selector);
			print(" => ");
			printHex(meth);
			print(")\n");
		}
	}
}