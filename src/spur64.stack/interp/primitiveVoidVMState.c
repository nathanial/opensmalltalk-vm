/* Extracted from interp.c:72123 (function primitiveVoidVMState). */

static void
primitiveVoidVMState(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt activeContext;
    char *sp;

	/* begin push: */
	longAtput((sp = GIV(stackPointer) - BytesPerWord),GIV(instructionPointer));
	GIV(stackPointer) = sp;
	activeContext = voidVMStateForSnapshotFlushingExternalPrimitivesIf(0);

	/* begin flushMethodCache */
	memset(GIV(methodCache), 0, MethodCacheSize * (sizeof(GIV(methodCache)[0])));

	/* this for primitiveExternalMethod */
	GIV(lastMethodCacheProbeWrite) = 0;

	/* begin flushAtCache */
	memset(GIV(atCache), 0, AtCacheTotalSize * (sizeof(GIV(atCache)[0])));
	marryContextInNewStackPageAndInitializeInterpreterRegisters(activeContext);
}