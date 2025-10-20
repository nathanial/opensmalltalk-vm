/* Extracted from B2DPlugin.c:7656 (function primitiveCopyBuffer). */

EXPORT(sqInt)
primitiveCopyBuffer(void) {
  sqInt buf1;
  sqInt buf2;
  sqInt diff;
  int *dst;
  sqInt failCode;
  sqInt i;
  int *src;
  int toDoLimit;
  int toDoLimit1;

  if (!((methodArgumentCount()) == 2)) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  buf2 = stackValue(0);
  buf1 = stackValue(1);

  /* Make sure the old buffer is properly initialized */
  if ((failCode = loadWorkBufferFrom(buf1))) {
    return primitiveFailFor(failCode);
  }

  /* Make sure the buffers are of the same type */
  if (!((fetchClassOf(buf1)) == (fetchClassOf(buf2)))) {
    return primitiveFailFor(GEFClassMismatch);
  }

  /* Make sure buf2 is at least of the size of buf1 */
  diff = (slotSizeOf(buf2)) - (slotSizeOf(buf1));
  if (diff < 0) {
    return primitiveFailFor(GEFSizeMismatch);
  }

  /* Okay - ready for copying. First of all just copy the contents up to wbTop
   */
  src = workBuffer;
  dst = firstIndexableField(buf2);
  toDoLimit = (workBuffer[GWBufferTop]) - 1;
  for (i = 0; i <= toDoLimit; i += 1) {
    dst[i] = (src[i]);
  }

  /* Adjust wbSize and wbTop in the new buffer */
  dst[GWBufferTop] = ((workBuffer[GWBufferTop]) + diff);
  dst[GWSize] = ((workBuffer[GWSize]) + diff);

  /* Now copy the entries from wbTop to wbSize */
  src += workBuffer[GWBufferTop];
  dst = (dst + (workBuffer[GWBufferTop])) + diff;
  toDoLimit1 = ((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) - 1;
  for (i = 0; i <= toDoLimit1; i += 1) {
    dst[i] = (src[i]);
  }

  /* Okay, done. Check the new buffer by loading the state from it */
  if ((failCode = loadWorkBufferFrom(buf2))) {
    return primitiveFailFor(failCode);
  }
  pop(2);
  return 0;
}