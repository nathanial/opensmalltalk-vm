/* Extracted from interp.c:72123 (function primitiveVoidVMState). */

/*	Void all internal VM state in the stack and machine code zones */

	/* StackInterpreterPrimitives>>#primitiveVoidVMState */

static void
primitiveVoidVMState(void)
{
    sqInt activeContext;
    char *sp;

	/* begin push: */
	longAtput((sp = stackPointer - BytesPerWord),instructionPointer);
	stackPointer = sp;
	activeContext = voidVMStateForSnapshotFlushingExternalPrimitivesIf(0);

	/* begin flushMethodCache */
	memset(methodCache, 0, MethodCacheSize * (sizeof(methodCache[0])));

	/* this for primitiveExternalMethod */
	lastMethodCacheProbeWrite = 0;

	/* begin flushAtCache */
	memset(atCache, 0, AtCacheTotalSize * (sizeof(atCache[0])));
	marryContextInNewStackPageAndInitializeInterpreterRegisters(activeContext);
}