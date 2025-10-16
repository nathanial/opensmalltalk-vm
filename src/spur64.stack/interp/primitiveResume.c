/* Extracted from interp.c:21461 (function primitiveResume). */

static void
primitiveResume(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt ctxt;
    sqInt proc;

	/* rcvr */
	/* Alas in Spur we need a read barrier */
	proc = longAt(GIV(stackPointer));

	/* begin followObjField:ofObject: */
	ctxt = longAt((void *)((proc + BaseHeaderSize) + ((((usqInt)(SuspendedContextIndex) << (shiftForWord()))))));
	assert(isNonImmediate(ctxt));
	if ((!((longAt((void *)(ctxt))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
		ctxt = fixFollowedFieldofObjectwithInitialValue(SuspendedContextIndex, proc, ctxt);
	}
	if (!(/* isContext: */
			((!(ctxt & (tagMask()))))
		 && (((longAt((void *)(ctxt))) & (classIndexMask())) == ClassMethodContextCompactIndex))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadReceiver;
		return;
	}
	resumepreemptedYieldingIffrom(proc, GIV(preemptionYields), CSResume);
}