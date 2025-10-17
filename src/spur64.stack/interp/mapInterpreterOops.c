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
	if (shouldRemapObj(GIV(method))) {
		/* *rel to method */
		GIV(instructionPointer) -= GIV(method);
		GIV(method) = remapObj(GIV(method));
		GIV(instructionPointer) += GIV(method);
	}
	if (/* shouldRemapOop: */
		((!(GIV(newMethod) & (tagMask()))))
	 && (shouldRemapObj(GIV(newMethod)))) {
		GIV(newMethod) = remapObj(GIV(newMethod));
	}

	/* begin mapProfileState */
	/* begin profileStateDoUpdating: */
	if (GIV(profileProcess)) {
		if ((result = (shouldRemapObj(GIV(profileProcess))
					? remapObj(GIV(profileProcess))
					: 0))) {
			GIV(profileProcess) = result;
		}
	}
	if (GIV(profileMethod)) {
		if ((result = (shouldRemapObj(GIV(profileMethod))
					? remapObj(GIV(profileMethod))
					: 0))) {
			GIV(profileMethod) = result;
		}
	}
	if (GIV(profileSemaphore)) {
		if ((result = (shouldRemapObj(GIV(profileSemaphore))
					? remapObj(GIV(profileSemaphore))
					: 0))) {
			GIV(profileSemaphore) = result;
		}
	}
#  if LRPCheck
	sqLowLevelMFence();
	if (GIV(longRunningPrimitiveCheckMethod)) {
		if (GIV(longRunningPrimitiveCheckSequenceNumber) == GIV(statCheckForEvents)) {
			GIV(longRunningPrimitiveCheckMethod) = GIV(newMethod);
		}
		else {
			if (shouldRemapObj(GIV(longRunningPrimitiveCheckMethod))) {
				GIV(longRunningPrimitiveCheckMethod) = remapObj(GIV(longRunningPrimitiveCheckMethod));
			}
		}
		sqLowLevelMFence();
	}
	if (GIV(longRunningPrimitiveCheckSemaphore)) {
		if (shouldRemapObj(GIV(longRunningPrimitiveCheckSemaphore))) {
			GIV(longRunningPrimitiveCheckSemaphore) = remapObj(GIV(longRunningPrimitiveCheckSemaphore));
		}
	}
#  endif // LRPCheck

	if ((GIV(tempOop) != 0)
	 && (/* shouldRemapOop: */
		((!(GIV(tempOop) & (tagMask()))))
	 && (shouldRemapObj(GIV(tempOop))))) {
		GIV(tempOop) = remapObj(GIV(tempOop));
	}
	if ((GIV(tempOop2) != 0)
	 && (/* shouldRemapOop: */
		((!(GIV(tempOop2) & (tagMask()))))
	 && (shouldRemapObj(GIV(tempOop2))))) {
		GIV(tempOop2) = remapObj(GIV(tempOop2));
	}
}