/* Extracted from interp.c:38759 (function longPrintInstancesOf). */

void
longPrintInstancesOf(sqInt aClassOop)
{
    sqInt classIndex;

	classIndex = (long32At((void *)(aClassOop + 4))) & (identityHashHalfWordMask());
	if (classIndex != (isFreeObjectClassIndexPun())) {
		longPrintInstancesWithClassIndex(classIndex);
	}
}