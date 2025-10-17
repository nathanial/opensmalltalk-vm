/* Extracted from interp.c:50356 (function canContextSwitchIfActivatingheader).
 */

/*	Context switch should not be allowed on every method activation. In
        particular the
        implementation of ensure: and ifCurtailed: depends on there being no
        suspension point
        on failing primitive 198 (primitiveMarkUnwindMethod,
        primitiveMarkUnwindMethod). slowPrimitiveResponse used to state
        ``N.B. This means there is no suspension point on primitive failure
        which methods such as ensure: and ifCurtailed: rely on.''
        Rather than prevent context switch on all primitives but the ones we
        really need
        to be suspension points (primitiveSignal et al) we choose to allow
   context switch for all but primitiveMarkUnwindMethod and
        PrimNumberNoContextSwitch.  */

/* StackInterpreter>>#canContextSwitchIfActivating:header: */

sqInt canContextSwitchIfActivatingheader(sqInt theMethod, sqInt methodHeader) {
  usqInt firstBytecode;
  sqInt primitiveIndex;

  /* primitiveIndexOfMethod:header: */
  if (((methodHeader & AlternateHeaderHasPrimFlag) != 0)) {
    firstBytecode =
        (theMethod + ((LiteralStart + (((methodHeader >> 3)) &
                                       AlternateHeaderNumLiteralsMask)) *
                      BytesPerOop)) +
        BaseHeaderSize;
    primitiveIndex = (byteAt((void *)(firstBytecode + 1))) +
                     ((((usqInt)((byteAt((void *)(firstBytecode + 2)))) << 8)));
  } else {
    primitiveIndex = 0;
  }
  return (primitiveIndex != PrimNumberUnwindMarker) &&
         (primitiveIndex != PrimNumberNoContextSwitchMarker);
}