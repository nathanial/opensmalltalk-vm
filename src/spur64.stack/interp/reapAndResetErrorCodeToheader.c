/* Extracted from interp.c:61868 (function reapAndResetErrorCodeToheader). */

/*	Assuming the primFailCode is non-zero, check if the method consumes the
        error code
        and if so, assign it through theSP. Then zero the primFailCode. This is
        infrequent code,
        so keep it out of the common path. */

/* StackInterpreter>>#reapAndResetErrorCodeTo:header: */

static NoDbgRegParms NeverInline void
reapAndResetErrorCodeToheader(char *theSP, sqInt methodHeader) {
  usqInt initialPC;

  assert(primFailCode != 0);
  initialPC = (((((usqInt)(pointerForOop(newMethod)))) +
                ((LiteralStart +
                  ((/* begin literalCountOfMethodHeader: */
                    assert((((methodHeader) & 7) == 1)),
                    /* literalCountOfAlternateHeader: */
                    ((methodHeader >> 3)) & AlternateHeaderNumLiteralsMask))) *
                 BytesPerOop)) +
               BaseHeaderSize) +
              3 /* sizeOfCallPrimitiveBytecode: */;
  if ((byteAt((void *)(initialPC))) ==
      (/* longStoreBytecodeForHeader: */
       ((((sqLong)methodHeader)) < 0 ? AltLongStoreBytecode
                                     : LongStoreBytecode))) {
    longAtput(theSP, getErrorObjectFromPrimFailCode());
  }
  primFailCode = 0;
}