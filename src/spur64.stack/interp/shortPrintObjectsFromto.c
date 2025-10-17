/* Extracted from interp.c:45568 (function shortPrintObjectsFromto). */

/*	useful for VM debugging */

	/* SpurMemoryManager>>#shortPrintObjectsFrom:to: */

void
shortPrintObjectsFromto(sqInt startAddress, sqInt endAddress)
{
    sqInt oop;

	oop = objectBefore(startAddress);
	oop = (oop
				? ((objectAfter(oop)) == startAddress
						? startAddress
						: oop)
				: startAddress);
	while (oopisLessThan(oop, endAddress)) {
		if (!(((longAt((void *)(oop))) & (classIndexMask())) == (isFreeObjectClassIndexPun()))) {
			shortPrintOop(oop);
		}
		oop = objectAfter(oop);
	}
}