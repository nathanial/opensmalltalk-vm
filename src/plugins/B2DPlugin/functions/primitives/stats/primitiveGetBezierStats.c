/* Extracted from B2DPlugin.c:7905 (function primitiveGetBezierStats). */

EXPORT(sqInt)
primitiveGetBezierStats(void) {
  sqInt failureCode;
  sqInt statOop;
  int *stats;

  if (!((methodArgumentCount()) == 1)) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  if ((failureCode = quickLoadEngineFrom(stackValue(1)))) {
    return primitiveFailFor(failureCode);
  }
  statOop = stackObjectValue(0);
  if (!((!(failed())) &&
        ((isWords(statOop)) && ((slotSizeOf(statOop)) >= 4)))) {
    return primitiveFailFor(PrimErrBadArgument);
  }
  stats = firstIndexableField(statOop);
  stats[0] = ((stats[0]) + (workBuffer[GWBezierMonotonSubdivisions]));
  stats[1] = ((stats[1]) + (workBuffer[GWBezierHeightSubdivisions]));
  stats[2] = ((stats[2]) + (workBuffer[GWBezierOverflowSubdivisions]));
  stats[3] = ((stats[3]) + (workBuffer[GWBezierLineConversions]));
  pop(1);
  return 0;
}