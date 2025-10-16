/* Extracted from interp.c:11249 (function isPositiveMachineIntegerObject). */

sqInt
isPositiveMachineIntegerObject(sqInt oop)
{
    sqInt ccIndex;
    int ok;

	if ((((oop) & 7) == 1)) {
		return ((oop >> 3)) >= 0;
	}
	if (((oop & (tagMask())) != 0)) {
		return 0;
	}

	/* begin isClassOfNonImm:equalTo:compactClassIndex: */
	assert(!(isImmediate(oop)));
	ccIndex = (longAt((void *)(oop))) & (classIndexMask());
	ok = ClassLargePositiveIntegerCompactIndex == ccIndex;
	return ok
	 && ((numBytesOfBytes(oop)) <= (sizeof(usqIntptr_t)));
}