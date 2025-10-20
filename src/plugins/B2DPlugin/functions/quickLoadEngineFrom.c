/* Extracted from B2DPlugin.c:9328 (function quickLoadEngineFrom). */

static sqInt quickLoadEngineFrom(sqInt engineOop) {
  sqInt failCode;

  if (failed()) {
    return GEFAlreadyFailed;
  }
  if (isImmediate(engineOop)) {
    return GEFEngineIsInteger;
  }
  if (!(isPointers(engineOop))) {
    return GEFEngineIsWords;
  }
  if ((slotSizeOf(engineOop)) < BEBalloonEngineSize) {
    return GEFEngineTooSmall;
  }
  engine = engineOop;
  if ((failCode = loadWorkBufferFrom(
           fetchPointerofObject(BEWorkBufferIndex, engineOop)))) {
    return failCode;
  }

  /* stopReasonPut: */
  workBuffer[GWStopReason] = 0;
  objUsed = workBuffer[GWObjUsed];
  engineStopped = 0;
  return 0;
}