/* Extracted from interp.c:68214 (function primitiveExternalCall). */

/*	Call an external primitive. External primitive methods first literals
   are an array of
        * The module name (String | Symbol)
        * The function name (String | Symbol)
        * The session ID (SmallInteger) [OBSOLETE], or in Spur, the metadata
        (accessorDepth and flags; Integer))
        * The function index (Integer) in the externalPrimitiveTable
        For fast interpreter dispatch in subsequent invocations the
        primitiveFunctionPointer in the method cache is rewritten, either to the
        function itself, or to zero if the external
        function is not found. This allows for fast responses as long as the
        method stays in
        the cache. The cache rewrite relies on lastMethodCacheProbeWrite which
   is set in addNewMethodToCache:. Now that the VM flushes function addresses
   from its tables, the session ID is obsolete, but it is kept for backward
   compatibility. Also, a failed lookup is reported specially. If a method has
   been looked up and not been found, the function address is stored as -1
        (i.e., the SmallInteger -1 to distinguish from 16rFFFFFFFF which may be
        returned from
        lookup), and the primitive fails with PrimErrNotFound.
 */
/*	because the primitive accesses newMethod's first literal, which is
   checked for explicitly in checkForAndFollowForwardedPrimitiveState
 */
/*	since call may invoke a callback */
/*	since external primitive linkage is in first literal... */
/*	Check for it being a method for primitiveDoPrimitiveWithArgs.
        Fetch the first literal of the method; check its an Array of length 4.
        Look at the function index in case it has been loaded before */

/* StackInterpreterPrimitives>>#primitiveExternalCall */

static void primitiveExternalCall(void) {
  void (*addr)(void);
  sqInt index;
  sqInt lit;
  usqInt numSlots;
  sqInt reasonCode;

  if (!((/* isOopCompiledMethod: */
         ((!(newMethod & (tagMask())))) &&
         (((byteAt((void *)(newMethod + (formatFieldByteOffset())))) &
           (formatMask())) >= (firstCompiledMethodFormat()))) &&
        (((literalCountOf(newMethod)) > 0) &&
         (((lit = fetchPointerofObject(1U, newMethod)),
           (/* isArray: */
            ((!(lit & (tagMask())))) &&
            (((byteAt((void *)(lit + (formatFieldByteOffset())))) &
              (formatMask())) == (arrayFormat()))) &&
               ((((/* begin numSlotsOf: */
                   assert((classIndexOf(lit)) >
                          (isForwardedObjectClassIndexPun())),
                   (((numSlots = byteAt(
                          (void *)(lit + (numSlotsFieldByteOffset()))))) ==
                            (numSlotsMask())
                        ? ((((usqInt)((
                              (sqInt)((usqInt)((longAt(
                                          (void *)(lit - BaseHeaderSize))))
                                      << 8)))))) >>
                              8
                        : numSlots))) == 4) &&
                (((index = longAt((
                       void
                           *)((lit + BaseHeaderSize) +
                              ((((usqInt)(ExternalCallLiteralTargetFunctionIndex)
                                 << (shiftForWord()))))))),
                  (((index) & 7) == 1))))))))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadMethod;
    return;
  }
  index = (index >> 3);

  /* Check if we have already looked up the function and failed. */
  if (index < 0) {
    rewriteMethodCacheEntryForExternalPrimitiveToFunction(0);

    /* primitiveFailFor: */
    primFailCode = PrimErrNotFound;
    return;
  }

  /* Function address was not found in this session,
     Void the primitive function.
     Try to call the function directly */
  if ((index > 0) && (index <= MaxExternalPrimitiveTableSize)) {
    addr = externalPrimitiveTable[index - 1];
    if (addr) {
      rewriteMethodCacheEntryForExternalPrimitiveToFunction(addr);

      /* begin callExternalPrimitive: */
      primitiveFunctionPointer = addr;
      dispatchFunctionPointer(addr);

      /* begin maybeRetryPrimitiveOnFailure */
      if (primFailCode) {
        retryPrimitiveOnFailure();
      }
      return;
    }

    /* if we get here, then an index to the external prim was
       kept on the ST side although the underlying prim
       table was already flushed */

    /* primitiveFailFor: */
    primFailCode = PrimErrNamedInternal;
    return;
  }

  /* Clean up session id/metadata and external primitive index */

  /* begin storePointerUnchecked:ofObject:withValue: */
  assert((isNonImmediate(lit)) && (!(isForwarded(lit))));
  assert(validStorePointerUncheckedArgs(ExternalCallLiteralFlagsIndex, lit,
                                        ConstZero));
  longAtput((void *)((lit + BaseHeaderSize) +
                     ((((usqInt)(ExternalCallLiteralFlagsIndex)
                        << (shiftForWord()))))),
            ConstZero);

  /* begin storePointerUnchecked:ofObject:withValue: */
  assert((isNonImmediate(lit)) && (!(isForwarded(lit))));
  assert(validStorePointerUncheckedArgs(ExternalCallLiteralTargetFunctionIndex,
                                        lit, ConstZero));
  longAtput((void *)((lit + BaseHeaderSize) +
                     ((((usqInt)(ExternalCallLiteralTargetFunctionIndex)
                        << (shiftForWord()))))),
            ConstZero);

  /* The function has not been loaded yet. Attempt to link it, cache it, and
   * call it. */
  addr = linkExternalCallerrInto(lit, (&primFailCode));
  if (!addr) {
    assert((fetchPointerofObject(ExternalCallLiteralFlagsIndex, lit)) ==
           ConstZero);
    reasonCode = (primFailCode ? primFailCode : PrimErrNotFound);
    primFailCode = reasonCode;
    return;
  }

  /* begin callExternalPrimitive: */
  primitiveFunctionPointer = addr;
  dispatchFunctionPointer(addr);

  /* begin maybeRetryPrimitiveOnFailure */
  if (primFailCode) {
    retryPrimitiveOnFailure();
  }
}