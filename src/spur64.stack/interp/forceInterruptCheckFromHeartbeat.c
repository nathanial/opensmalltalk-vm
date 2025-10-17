/* Extracted from interp.c:53902 (function forceInterruptCheckFromHeartbeat). */

/*	Force an interrupt check ASAP. This version is the
	entry-point to forceInterruptCheck for the heartbeat
	timer to allow for repeatable debugging. */

	/* StackInterpreter>>#forceInterruptCheckFromHeartbeat */

void
forceInterruptCheckFromHeartbeat(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	if (!suppressHeartbeatFlag) {
		/* begin checkForLongRunningPrimitive */
#    if LRPCheck
		if (!GIV(longRunningPrimitiveCheckSemaphore)) {
			goto l1;
		}
		if ((GIV(longRunningPrimitiveStartUsecs) > 0)
		 && ((GIV(longRunningPrimitiveCheckMethod) == GIV(newMethod))
		 && (GIV(longRunningPrimitiveCheckSequenceNumber) == GIV(statCheckForEvents)))) {
			GIV(longRunningPrimitiveStopUsecs) = ioUTCMicroseconds();
			assert(GIV(longRunningPrimitiveStopUsecs) > GIV(longRunningPrimitiveStartUsecs));
			goto l1;
		}

		/* See traceProfileState & mapProfileState. */
		if (!GIV(longRunningPrimitiveStopUsecs)) {
			GIV(longRunningPrimitiveCheckSequenceNumber) = GIV(statCheckForEvents);
			GIV(longRunningPrimitiveCheckMethod) = GIV(newMethod);
			GIV(longRunningPrimitiveStartUsecs) = ioUTCMicroseconds();
			sqLowLevelMFence();
		}
#    endif // LRPCheck

		/* end checkForLongRunningPrimitive */
l1:
		forceInterruptCheck();
	}
}