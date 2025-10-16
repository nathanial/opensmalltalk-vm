/* Extracted from interp.c:11205 (function isNegativeIntegerValueOf). */

static NoDbgRegParms sqInt
isNegativeIntegerValueOf(sqInt oop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt ccIndex;
    int ok;
    sqInt smallInt;

	if ((((oop) & 7) == 1)) {
		smallInt = (oop >> 3);
		return smallInt < 0;
	}
	if ((!(oop & (tagMask())))) {
		/* begin isClassOfNonImm:equalTo:compactClassIndex: */
		assert(!(isImmediate(oop)));
		ccIndex = (longAt((void *)(oop))) & (classIndexMask());
		ok = ClassLargePositiveIntegerCompactIndex == ccIndex;
		if (ok) {
			return 0;
		}

		/* begin isClassOfNonImm:equalTo:compactClassIndex: */
		assert(!(isImmediate(oop)));
		ccIndex = (longAt((void *)(oop))) & (classIndexMask());
		ok = ClassLargeNegativeIntegerCompactIndex == ccIndex;
		if (ok) {
			return 1;
		}
	}

	/* begin primitiveFail */
	if (!GIV(primFailCode)) {
		GIV(primFailCode) = 1;
	}
	return 0;
}