/* Extracted from interp.c:24030 (function primitiveSnapshot). */

/*	Save a normal snapshot under the same name as it was loaded unless it
   has been renamed by the last primitiveImageName. Note that when executed this
   primitive answers false, but when the resulting image is run afresh, the
   primitive answers true.
 */

/* InterpreterPrimitives>>#primitiveSnapshot */

static void primitiveSnapshot(void) { snapshot(0); }