/* Extracted from interp.c:46899 (function whereIsMaybeHeapThing). */

	/* SpurMemoryManager>>#whereIsMaybeHeapThing: */

static NoDbgRegParms char *
whereIsMaybeHeapThing(sqInt anOop)
{
	if (/* isInNewSpace: */
		(oopisLessThan(anOop, oldSpaceStart))
	 && (oopisGreaterThanOrEqualTo(anOop, newSpaceStart))) {
		if (oopisGreaterThanOrEqualToandLessThan(anOop, ((eden).start), freeStart)) {
			return " is in eden";
		}
		if (oopisGreaterThanOrEqualToandLessThan(anOop, ((futureSpace).start), futureSurvivorStart)) {
			return " is in future space";
		}
		if (oopisGreaterThanOrEqualToandLessThan(anOop, ((pastSpace).start), pastSpaceStart)) {
			return " is in past space";
		}
		return " is in new space";
	}
	if (oopisGreaterThanOrEqualToandLessThan(anOop, oldSpaceStart, endOfMemory)) {
		if (segmentContainingObj(anOop)) {
			return " is in old space";
		}
		return " is between old space segments";
	}
	return null;
}