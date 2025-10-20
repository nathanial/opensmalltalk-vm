/* Extracted from B2DPlugin.c:5074 (function loadPointfrom). */

static sqInt loadPointfrom(int *pointArray, sqInt pointOop) {
  sqInt value;

  if (!((fetchClassOf(pointOop)) == (classPoint()))) {
    return primitiveFail();
  }
  value = fetchPointerofObject(0, pointOop);
  if (!((isIntegerObject(value)) || (isFloatObject(value)))) {
    return primitiveFail();
  }
  if (isIntegerObject(value)) {
    pointArray[0] = (integerValueOf(value));
  } else {
    pointArray[0] = (((sqInt)(floatValueOf(value))));
  }
  value = fetchPointerofObject(1, pointOop);
  if (!((isIntegerObject(value)) || (isFloatObject(value)))) {
    return primitiveFail();
  }
  if (isIntegerObject(value)) {
    pointArray[1] = (integerValueOf(value));
  } else {
    pointArray[1] = (((sqInt)(floatValueOf(value))));
  }
  return 0;
}