/* Extracted from interp.c:63568 (function slowPrimitiveResponse). */

/*	Invoke a normal (non-quick) primitive.
        Called under the assumption that primFunctionPointer has been preloaded.
 */

/* StackInterpreter>>#slowPrimitiveResponse */

static int slowPrimitiveResponse(void) {
  assert(!(isOopForwarded(stackValue(argumentCount))));
  assert((remapBufferCount) == 0);

  /* begin initPrimCall */
  primFailCode = 0;
  if (LOGPRIMITIVES) {
    /* begin fastLogPrim: */
    primTraceLog[primTraceLogIndex] = newMethod;
    primTraceLogIndex(primTraceLogIndex + 1);
  }
  dispatchFunctionPointer(primitiveFunctionPointer);
  assert(maybeLeakCheckExternalPrimCall(newMethod));

  /* begin maybeRetryPrimitiveOnFailure */
  if (primFailCode) {
    retryPrimitiveOnFailure();
  }

  /* Don't fail if primitive has done something radical, e.g. perform:
     If we are profiling, take accurate primitive measures */
  if (nextProfileTick > 0) {
    checkProfileTickPostPrimitive(newMethod);
  }
  return !primFailCode;
}