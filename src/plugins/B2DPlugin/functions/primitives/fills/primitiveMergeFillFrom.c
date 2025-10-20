/* Extracted from B2DPlugin.c:8224 (function primitiveMergeFillFrom). */

EXPORT(sqInt)
primitiveMergeFillFrom(void) {
  sqInt bitsOop;
  sqInt failureCode;
  sqInt fillOop;
  sqInt value;
  sqInt valueSqInt;

  if (doProfileStats) {
    geProfileTime = ioMicroMSecs();
  }
  if (!((methodArgumentCount()) == 2)) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  if ((failureCode = quickLoadEngineFromrequiredState(stackValue(2),
                                                      GEStateWaitingForFill))) {
    return primitiveFailFor(failureCode);
  }

  /* Load span buffer for merging the fill */
  if ((failureCode =
           loadSpanBufferFrom(fetchPointerofObject(BESpanIndex, engine)))) {
    return primitiveFailFor(failureCode);
  }
  fillOop = stackObjectValue(0);
  bitsOop = stackObjectValue(1);

  /* Check bitmap */
  if (!((!(failed())) && ((fetchClassOf(bitsOop)) == (classBitmap())))) {
    return primitiveFailFor(PrimErrBadArgument);
  }

  /* Check fillOop */
  if ((slotSizeOf(fillOop)) < FTBalloonFillDataSize) {
    return primitiveFailFor(GEFFillDataTooSmall);
  }

  /* Check if this was the fill we have exported */
  value = fetchIntegerofObject(FTIndexIndex, fillOop);
  if (!((objBuffer[(workBuffer[GWLastExportedFill]) + GEObjectIndex]) ==
        value)) {
    return primitiveFailFor(GEFWrongFill);
  }
  value = fetchIntegerofObject(FTMinXIndex, fillOop);
  if (!((workBuffer[GWLastExportedLeftX]) == value)) {
    return primitiveFailFor(GEFWrongFill);
  }
  value = fetchIntegerofObject(FTMaxXIndex, fillOop);
  if (!((workBuffer[GWLastExportedRightX]) == value)) {
    return primitiveFailFor(GEFWrongFill);
  }
  if ((slotSizeOf(bitsOop)) < ((workBuffer[GWLastExportedRightX]) -
                               (workBuffer[GWLastExportedLeftX]))) {
    return primitiveFailFor(PrimErrBadArgument);
  }
  if (failed()) {
    return null;
  }
  fillBitmapSpanfromto(((int *)(firstIndexableField(bitsOop))),
                       workBuffer[GWLastExportedLeftX],
                       workBuffer[GWLastExportedRightX]);

  /* statePut: */
  workBuffer[GWState] = GEStateScanningAET;

  /* begin storeEngineStateInto: */
  /* objUsedPut: */
  workBuffer[GWObjUsed] = objUsed;
  pop(2);
  if (doProfileStats) {
    /* incrementStat:by: */
    workBuffer[GWCountMergeFill] = ((workBuffer[GWCountMergeFill]) + 1);
    valueSqInt = (ioMicroMSecs()) - geProfileTime;

    /* begin incrementStat:by: */
    workBuffer[GWTimeMergeFill] = ((workBuffer[GWTimeMergeFill]) + valueSqInt);
  }
  return 0;
}