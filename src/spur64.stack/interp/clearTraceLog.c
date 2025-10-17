/* Extracted from interp.c:51479 (function clearTraceLog). */

	/* StackInterpreter>>#clearTraceLog */

void
clearTraceLog(void)
{
    sqInt i;

	traceLogIndex = 0;
	for (i = 0; i < TraceBufferSize; i += 1) {
		traceLog[i] = 0;
	}
}