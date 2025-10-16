/* Extracted from interp.c:16472 (function primitiveForceDisplayUpdate). */

/*	On some platforms, this primitive forces enqueued display updates to be
        processed immediately. On others, it does nothing.
 */

/* InterpreterPrimitives>>#primitiveForceDisplayUpdate */

static void primitiveForceDisplayUpdate(void) { ioForceDisplayUpdate(); }