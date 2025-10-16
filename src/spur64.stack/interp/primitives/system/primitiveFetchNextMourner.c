/* Extracted from interp.c:15401 (function primitiveFetchNextMourner). */

/* InterpreterPrimitives>>#primitiveFetchNextMourner */

static void primitiveFetchNextMourner(void) {
  sqInt mourner;

  mourner = /* dequeueMourner */
      (mournQueue != nilObj
           ? (assert(isValidObjStack(mournQueue)), popObjStack(mournQueue))
           : 0);
  if (mourner) {
    assert(isValidObjStack(mournQueue));

    popthenPush(1, mourner);
  } else {
    /* primitiveFailFor: */
    primFailCode = PrimErrNotFound;
  }
}
