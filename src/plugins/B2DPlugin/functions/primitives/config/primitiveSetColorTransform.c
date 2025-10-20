/* Extracted from B2DPlugin.c:8870 (function primitiveSetColorTransform). */

EXPORT(sqInt)
primitiveSetColorTransform(void) {
  sqInt failureCode;
  sqInt okay;
  float *transform;
  sqInt transformOop;

  if (!((methodArgumentCount()) == 1)) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  if ((failureCode =
           quickLoadEngineFromrequiredState(stackValue(1), GEStateUnlocked))) {
    return primitiveFailFor(failureCode);
  }
  transformOop = stackObjectValue(0);
  if (failed()) {
    return primitiveFailFor(PrimErrBadArgument);
  }

  /* begin loadColorTransformFrom: */
  /* begin colorTransform */
  transform = ((float *)(workBuffer + GWColorTransform));

  /* hasColorTransformPut: */
  workBuffer[GWHasColorTransform] = 0;
  okay = loadTransformFromintolength(transformOop, transform, 8);
  if (okay) {
    /* hasColorTransformPut: */
    workBuffer[GWHasColorTransform] = 1;

    /* Scale transform to be in 0-256 range */
    transform[1] = ((transform[1]) * (256.0f));
    transform[3] = ((transform[3]) * (256.0f));
    transform[5] = ((transform[5]) * (256.0f));
    transform[7] = ((transform[7]) * (256.0f));
  }
  /* end loadColorTransformFrom: */
  if (failed()) {
    return primitiveFailFor(GEFEntityLoadFailed);
  }

  /* begin storeEngineStateInto: */
  /* objUsedPut: */
  workBuffer[GWObjUsed] = objUsed;
  pop(1);
  return 0;
}