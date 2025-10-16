/* Extracted from interp.c:18399 (function primitiveInterruptChecksPerMSec). */

EXPORT(sqInt)
primitiveInterruptChecksPerMSec(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt oop;
    char *sp;

	if (GIV(argumentCount)) {
		/* begin success: */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		return null;
	}
	oop = (((usqInt)(ioHeartbeatMilliseconds()) << 3) | 1);

	/* begin pop:thenPush: */
	longAtput((sp = GIV(stackPointer)),oop);
	GIV(stackPointer) = sp;
	return 0;
}