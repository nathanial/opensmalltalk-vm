/* Extracted from interp.c:46899 (function whereIsMaybeHeapThing). */

	/* SpurMemoryManager>>#whereIsMaybeHeapThing: */

static NoDbgRegParms char *
whereIsMaybeHeapThing(sqInt anOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	if (/* isInNewSpace: */
		(oopisLessThan(anOop, GIV(oldSpaceStart)))
	 && (oopisGreaterThanOrEqualTo(anOop, GIV(newSpaceStart)))) {
		if (oopisGreaterThanOrEqualToandLessThan(anOop, ((GIV(eden)).start), GIV(freeStart))) {
			return " is in eden";
		}
		if (oopisGreaterThanOrEqualToandLessThan(anOop, ((GIV(futureSpace)).start), GIV(futureSurvivorStart))) {
			return " is in future space";
		}
		if (oopisGreaterThanOrEqualToandLessThan(anOop, ((GIV(pastSpace)).start), GIV(pastSpaceStart))) {
			return " is in past space";
		}
		return " is in new space";
	}
	if (oopisGreaterThanOrEqualToandLessThan(anOop, GIV(oldSpaceStart), GIV(endOfMemory))) {
		if (segmentContainingObj(anOop)) {
			return " is in old space";
		}
		return " is between old space segments";
	}
	return null;
}