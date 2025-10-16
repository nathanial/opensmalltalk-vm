/* Extracted from interp.c:63568 (function slowPrimitiveResponse). */

static int
slowPrimitiveResponse(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	assert(!(isOopForwarded(stackValue(GIV(argumentCount)))));
	assert((GIV(remapBufferCount)) == 0);

	/* begin initPrimCall */
	GIV(primFailCode) = 0;
	if (LOGPRIMITIVES) {
		/* begin fastLogPrim: */
		GIV(primTraceLog)[GIV(primTraceLogIndex)] = GIV(newMethod);
		primTraceLogIndex(GIV(primTraceLogIndex) + 1);
	}
	dispatchFunctionPointer(primitiveFunctionPointer);
	assert(maybeLeakCheckExternalPrimCall(GIV(newMethod)));

	/* begin maybeRetryPrimitiveOnFailure */
	if (GIV(primFailCode)) {
		retryPrimitiveOnFailure();
	}

	/* Don't fail if primitive has done something radical, e.g. perform:
	   If we are profiling, take accurate primitive measures */
	if (GIV(nextProfileTick) > 0) {
		checkProfileTickPostPrimitive(GIV(newMethod));
	}
	return !GIV(primFailCode);
}