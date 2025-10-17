/* Extracted from interp.c:62757 (function
 * saneFunctionPointerForFailureOfPrimIndex). */

/*	This is an assert function used to ensure consistency between the
        primitiveFunctionPointer and the primitive index when a primitive fails
   in Spur. Since Spur automagically retries failing primitives we want to know
   that the primitiveFunctionPointer is actually valid. This isn't always
   possible for the `indirect'' primitives (calling a plugin primitive, calling
   the ffi, using the receiver:tryPrimitive:withArgs: primitive evaluator in the
   simulator. We do the best we can. */

/* StackInterpreter>>#saneFunctionPointerForFailureOfPrimIndex: */

static sqInt saneFunctionPointerForFailureOfPrimIndex(sqInt primIndex) {
  void (*basePrimitive)(void);

  /* begin functionPointerFor:inClass: */
  basePrimitive =
      ((((usqInt)primIndex)) > MaxPrimitiveIndex ? ((void (*)(void))0)
                                                 : primitiveTable[primIndex]);
  return (primitiveFunctionPointer == basePrimitive) ||
         (((/* isCalloutPrimitiveIndex: */
            (primIndex == PrimNumberExternalCall) ||
            (primIndex == PrimNumberFFICall)) &&
           (!((((usqIntptr_t)primitiveFunctionPointer)) <=
              MaxQuickPrimitiveIndex))) ||
          ((/* isMetaPrimitiveIndex: */
            (primIndex == PrimNumberDoPrimitive) ||
            (primIndex == PrimNumberDoExternalCall)) &&
           (metaAccessorDepth > -2)));
}