/* Extracted from interp.c:72113 (function primitiveVoidReceiver). */

EXPORT(void)
primitiveVoidReceiver(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	longAtput(GIV(framePointer) + FoxReceiver,0);
}