/* Extracted from interp.c:72113 (function primitiveVoidReceiver). */

/*	Potentially crash the VM by voiding the receiver. A subsequent inst var
	access in the caller's frame should indirect through a null pointer. */

	/* StackInterpreterPrimitives>>#primitiveVoidReceiver */

EXPORT(void)
primitiveVoidReceiver(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	longAtput(framePointer + FoxReceiver,0);
}