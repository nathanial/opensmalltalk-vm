/* Extracted from interp.c:21228 (function primitiveProfileStart). */

/*	Primitive. Begin profiling execution by using the high-resolution clock
	instead of a time-based process
	(which is limited to timing resolution and triggers off the same signal
	that many of the processes being
	profiled trigger off leading to consistently wrong results). The argument
	is the number of ticks of the
	high-resolution clock to elapse before taking a sample. The sample is
	stored in the profileProcess and
	profileMethod iVars which can be retrieved via primitiveProfileSample and
	primitiveProfilePrimitive. So also void the sample iVars. Once a sample is
	taken, the semaphore installed via
	primitiveProfileSemaphore is signalled. If the argument is less or equal
	to zero, profiling is disabled. */

	/* InterpreterPrimitives>>#primitiveProfileStart */

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