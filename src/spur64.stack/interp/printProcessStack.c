/* Extracted from interp.c:60810 (function printProcessStack). */

void
printProcessStack(sqInt aProcess)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt ctx;
    sqInt tagBits;

	cr();
	printNameOfClasscount(/* fetchClassOf: */
		((tagBits = aProcess & (tagMask()))
			? longAt((void *)((GIV(classTableFirstPage) + BaseHeaderSize) + ((((usqInt)(tagBits) << (shiftForWord()))))))
			: fetchClassOfNonImm(aProcess)), 5);

	/* begin space */
	printChar(' ');
	printHex(aProcess);
	print(" priority ");
	printNum(quickFetchIntegerofObject(PriorityIndex, aProcess));
	cr();
	ctx = followFieldofObject(SuspendedContextIndex, aProcess);
	if (!(ctx == GIV(nilObj))) {
		printCallStackOfcurrentFP(ctx, GIV(framePointer));
	}
}