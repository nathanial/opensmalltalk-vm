/* Extracted from interp.c:18399 (function primitiveInterruptChecksPerMSec). */

/*	Primitive. Answer the number of interrupt checks per milliseconds that
   we execute on this machine. This can be used to adjust the sub-msecs profiler
        to check (roughly)
        n times per millisecond.
 */

/* InterpreterPrimitives>>#primitiveInterruptChecksPerMSec */

EXPORT(sqInt)
primitiveInterruptChecksPerMSec(void) {
  sqInt oop;

  if (argumentCount) {
    /* begin success: */
    if (!primFailCode) {
      primFailCode = 1;
    }
    return null;
  }
  oop = (((usqInt)(ioHeartbeatMilliseconds()) << 3) | 1);

  popthenPush(1, oop);
  return 0;
}
