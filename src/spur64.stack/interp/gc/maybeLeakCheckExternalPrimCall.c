/* Extracted from interp.c:57633 (function maybeLeakCheckExternalPrimCall). */

/*	Assert-only check for leaks after external prim calls if checkForLeaks
        includes the GCCheckPrimCall flag.
        This is ionly really useful from a low-level C debugger, hence no
        accessors for checkedPluginName.
 */

/* StackInterpreter>>#maybeLeakCheckExternalPrimCall: */

static sqInt maybeLeakCheckExternalPrimCall(sqInt aMethodObj) {
  if ((((checkForLeaks & GCCheckPrimCall) != 0)) &&
      (((primitiveIndexOf(aMethodObj)) == PrimNumberExternalCall) &&
       (checkedPluginName ==
        (fetchPointerofObject(0,
                              fetchPointerofObject(1U, aMethodObj)))))) {
    runLeakCheckerFor(GCCheckPrimCall);
  }
  return 1;
}