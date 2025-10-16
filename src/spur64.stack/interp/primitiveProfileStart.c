/* Extracted from interp.c:21228 (function primitiveProfileStart). */

EXPORT(sqInt)
primitiveProfileStart(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt deltaTicks;

	deltaTicks = longAt(GIV(stackPointer));
	if ((((deltaTicks) & 7) == 1)) {
		GIV(deferProfileCheckForVNCS) = 0;
		GIV(profileProcess) = (GIV(profileMethod) = null);
		deltaTicks = (deltaTicks >> 3);
		GIV(nextProfileTick) = (deltaTicks > 0
					? (ioHighResClock()) + deltaTicks
					: 0);

		/* begin methodReturnReceiver */
		assert(!((failed())));
		GIV(stackPointer) += GIV(argumentCount) * BytesPerWord;
		return 0;
	}
	return (GIV(primFailCode) = PrimErrBadArgument);
}