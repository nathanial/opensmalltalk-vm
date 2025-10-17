/* Extracted from interp.c:21461 (function primitiveResume). */

/*	Put this process on the scheduler's lists thus allowing it to proceed next
	time there is
	a chance for processes of it's priority level. It must go to the back of
	its run queue so
	as not to preempt any already running processes at this level. If the
	process's priority
	is higher than the current process, preempt the current process. */

	/* InterpreterPrimitives>>#primitiveResume */

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