/* Extracted from interp.c:23987 (function primitiveSmallFloatTruncated). */

/* InterpreterPrimitives>>#primitiveSmallFloatTruncated */

static void primitiveSmallFloatTruncated(void) {
  usqLong bits;
  sqInt oop;
  double rcvr;
  double trunc;

  oop = longAt(stackPointer);

  rcvr = smallFloatValueOf(oop);
  modf(rcvr, (&trunc));
  if ((((trunc >= (((double)(MinSmallInteger)))) &&
        (trunc <= (((double)(MaxSmallInteger)))))) &&
      ((((((usqInt)((((sqInt)trunc)))) >> 60) + 1) & 15) <= 1)) {
    /* stackTopPut: */
    longAtput(stackPointer, (((usqInt)(((sqInt)trunc)) << 3) | 1));
  } else {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
  }
}