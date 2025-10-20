/* Extracted from B2DPlugin.c:5318 (function loadTransformFromArrayintolength). */

static sqInt loadTransformFromArrayintolength(sqInt transformOop,
                                              float *destPtr, sqInt n) {
  sqInt i;
  sqInt value;

  for (i = 0; i < n; i += 1) {
    value = fetchPointerofObject(i, transformOop);
    if (!((isIntegerObject(value)) || (isFloatObject(value)))) {
      return primitiveFail();
    }
    if (isIntegerObject(value)) {
      destPtr[i] = (((float)(((double)(integerValueOf(value))))));
    } else {
      destPtr[i] = (((float)(floatValueOf(value))));
    }
  }
  return 0;
}