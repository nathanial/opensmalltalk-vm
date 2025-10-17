/* Extracted from interp.c:51192 (function checkProfileTickPostPrimitive). */

/*	Check if the profile timer has expired during a primitive and if so take a
	sample. If the primitive has failed sample the profileMethod as nil. */
/*	If continuing from primitive[Full]ClosureValueNoContextSwitch defer until
	the next check
 */

	/* StackInterpreter>>#checkProfileTickPostPrimitive: */

static NoDbgRegParms NeverInline void
checkProfileTickPostPrimitive(sqInt aPrimitiveMethod)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt objOop;

	if (GIV(deferProfileCheckForVNCS)) {
		GIV(deferProfileCheckForVNCS) = 0;
	}
	else {
		if ((GIV(nextProfileTick) > 0)
		 && ((ioHighResClock()) >= GIV(nextProfileTick))) {
			if (!GIV(profileProcess)) {
				/* begin activeProcess */
				objOop = longAt((void *)(((longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(SchedulerAssociation) << (shiftForWord()))))))) + BaseHeaderSize) + ((((usqInt)(ValueIndex) << (shiftForWord()))))));
				GIV(profileProcess) = longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(ActiveProcessIndex) << (shiftForWord()))))));
				if (GIV(primFailCode)) {
					GIV(profileMethod) = null;
					forceInterruptCheck();
				}
				else {
					GIV(profileMethod) = aPrimitiveMethod;

					/* begin zeroNextProfileTick */
					GIV(nextProfileTick) = 0;
					synchronousSignal(GIV(profileSemaphore));
				}
			}
		}
	}
}