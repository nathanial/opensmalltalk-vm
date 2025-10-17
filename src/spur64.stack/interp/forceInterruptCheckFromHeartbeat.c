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
		if (!longRunningPrimitiveCheckSemaphore) {
			goto l1;
		}
		if ((longRunningPrimitiveStartUsecs > 0)
		 && ((longRunningPrimitiveCheckMethod == newMethod)
		 && (longRunningPrimitiveCheckSequenceNumber == statCheckForEvents))) {
			longRunningPrimitiveStopUsecs = ioUTCMicroseconds();
			assert(longRunningPrimitiveStopUsecs > longRunningPrimitiveStartUsecs);
			goto l1;
		}

		/* See traceProfileState & mapProfileState. */
		if (!longRunningPrimitiveStopUsecs) {
			longRunningPrimitiveCheckSequenceNumber = statCheckForEvents;
			longRunningPrimitiveCheckMethod = newMethod;
			longRunningPrimitiveStartUsecs = ioUTCMicroseconds();
			sqLowLevelMFence();
		}
#    endif // LRPCheck

		/* end checkForLongRunningPrimitive */
l1:
		forceInterruptCheck();
	}
}