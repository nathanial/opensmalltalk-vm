/* Extracted from interp.c:31746 (function beRootIfOld). */

void
beRootIfOld(sqInt oop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	/* begin isOldObject: */
	assert(isNonImmediate(oop));
	if (oopisGreaterThanOrEqualTo(oop, GIV(oldSpaceStart))) {
		/* begin possibleRootStoreInto: */
		if (!((byteAt((void *)(oop + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
			remember(oop);
		}
	}
}