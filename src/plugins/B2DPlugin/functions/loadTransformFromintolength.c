/* Extracted from B2DPlugin.c:5342 (function loadTransformFromintolength). */

static sqInt loadTransformFromintolength(sqInt transformOop, float *destPtr,
                                         sqInt n) {
  sqInt i;

  if (transformOop == (nilObject())) {
    return 0;
  }
  if (!((slotSizeOf(transformOop)) == n)) {
    return primitiveFail();
  }
  if (isWords(transformOop)) {
    /* begin loadTransformFromFloats:into:length: */
    for (i = 0; i < n; i += 1) {
      destPtr[i] = ((((float *)(firstIndexableField(transformOop))))[i]);
    }
  } else {
    loadTransformFromArrayintolength(transformOop, destPtr, n);
  }
  return 1;
}