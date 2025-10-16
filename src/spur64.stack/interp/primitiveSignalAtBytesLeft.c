/* Extracted from interp.c:22576 (function primitiveSignalAtBytesLeft). */

static void
primitiveSignalAtBytesLeft(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt bytes;

	bytes = longAt(GIV(stackPointer));
	if (((((bytes) & 7) == 1))
	 && (((bytes = (bytes >> 3))) >= 0)) {
		/* begin lowSpaceThreshold: */
		GIV(lowSpaceThreshold) = bytes;

		/* N.B. The threshold > 0 guard eliminates a warning when
		   self lowSpaceThreshold: 0
		   is inlined into setSignalLowSpaceFlagAndSaveProcess */
		if ((bytes > 0)
		 && (GIV(totalFreeOldSpace) < bytes)) {
			growOldSpaceByAtLeast(bytes - GIV(totalFreeOldSpace));
		}
		assert(GIV(totalFreeOldSpace) >= GIV(lowSpaceThreshold));

		/* begin pop: */
		GIV(stackPointer) += 1 * BytesPerWord;
	}
	else {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadArgument;
	}
}