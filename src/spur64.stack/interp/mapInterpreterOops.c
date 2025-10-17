/* Extracted from interp.c:56828 (function mapInterpreterOops). */

/*	Map all oops in the interpreter's state to their new values 
	during garbage collection or a become: operation. */
/*	Assume: All traced variables contain valid oops. */

	/* StackInterpreter>>#mapInterpreterOops */

static void
mapInterpreterOops(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt result;

	mapStackPages();

	/* begin mapTraceLogs */
	mapPrimTraceLog();
	mapTraceLog();

	/* begin mapVMRegisters */
	if (shouldRemapObj(method)) {
		/* *rel to method */
		instructionPointer -= method;
		method = remapObj(method);
		instructionPointer += method;
	}
	if (/* shouldRemapOop: */
		((!(newMethod & (tagMask()))))
	 && (shouldRemapObj(newMethod))) {
		newMethod = remapObj(newMethod);
	}

	/* begin mapProfileState */
	/* begin profileStateDoUpdating: */
	if (profileProcess) {
		if ((result = (shouldRemapObj(profileProcess)
					? remapObj(profileProcess)
					: 0))) {
			profileProcess = result;
		}
	}
	if (profileMethod) {
		if ((result = (shouldRemapObj(profileMethod)
					? remapObj(profileMethod)
					: 0))) {
			profileMethod = result;
		}
	}
	if (profileSemaphore) {
		if ((result = (shouldRemapObj(profileSemaphore)
					? remapObj(profileSemaphore)
					: 0))) {
			profileSemaphore = result;
		}
	}
#  if LRPCheck
	sqLowLevelMFence();
	if (longRunningPrimitiveCheckMethod) {
		if (longRunningPrimitiveCheckSequenceNumber == statCheckForEvents) {
			longRunningPrimitiveCheckMethod = newMethod;
		}
		else {
			if (shouldRemapObj(longRunningPrimitiveCheckMethod)) {
				longRunningPrimitiveCheckMethod = remapObj(longRunningPrimitiveCheckMethod);
			}
		}
		sqLowLevelMFence();
	}
	if (longRunningPrimitiveCheckSemaphore) {
		if (shouldRemapObj(longRunningPrimitiveCheckSemaphore)) {
			longRunningPrimitiveCheckSemaphore = remapObj(longRunningPrimitiveCheckSemaphore);
		}
	}
#  endif // LRPCheck

	if ((tempOop != 0)
	 && (/* shouldRemapOop: */
		((!(tempOop & (tagMask()))))
	 && (shouldRemapObj(tempOop)))) {
		tempOop = remapObj(tempOop);
	}
	if ((tempOop2 != 0)
	 && (/* shouldRemapOop: */
		((!(tempOop2 & (tagMask()))))
	 && (shouldRemapObj(tempOop2)))) {
		tempOop2 = remapObj(tempOop2);
	}
}