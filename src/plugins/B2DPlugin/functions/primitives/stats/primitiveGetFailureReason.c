/* Extracted from B2DPlugin.c:8012 (function primitiveGetFailureReason). */

EXPORT(sqInt)
primitiveGetFailureReason(void) {
  sqInt failCode;

  if (methodArgumentCount()) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  engine = stackValue(0);

  /* Note -- don't call loadEngineFrom here because this will override the
   * stopReason with Zero */
  if (isImmediate(engine)) {
    return primitiveFailFor(GEFEngineIsInteger);
  }
  if (!(isPointers(engine))) {
    return primitiveFailFor(GEFEngineIsWords);
  }
  if ((slotSizeOf(engine)) < BEBalloonEngineSize) {
    return primitiveFailFor(GEFEngineTooSmall);
  }
  if ((failCode = loadWorkBufferFrom(
           fetchPointerofObject(BEWorkBufferIndex, engine)))) {
    return primitiveFailFor(failCode);
  }
  pop(1);
  pushInteger(workBuffer[GWStopReason]);
  return 0;
}