/* Extracted from interp.c:18399 (function primitiveInterruptChecksPerMSec). */

/*	Primitive. Answer the number of interrupt checks per milliseconds that we
	execute on this machine. This can be used to adjust the sub-msecs profiler
	to check (roughly) 
	n times per millisecond.
 */

	/* InterpreterPrimitives>>#primitiveInterruptChecksPerMSec */

EXPORT(sqInt)
primitiveInterruptChecksPerMSec(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt oop;
    char *sp;

	if (argumentCount) {
		/* begin success: */
		if (!primFailCode) {
			primFailCode = 1;
		}
		return null;
	}
	oop = (((usqInt)(ioHeartbeatMilliseconds()) << 3) | 1);

	/* begin pop:thenPush: */
	longAtput((sp = stackPointer),oop);
	stackPointer = sp;
	return 0;
}