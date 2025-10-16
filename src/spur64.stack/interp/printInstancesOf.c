/* Extracted from interp.c:43480 (function printInstancesOf). */

void
printInstancesOf(sqInt aClassOop)
{
    sqInt classIndex;

	classIndex = (long32At((void *)(aClassOop + 4))) & (identityHashHalfWordMask());
	if (classIndex != (isFreeObjectClassIndexPun())) {
		printInstancesWithClassIndex(classIndex);
	}
}