/* Extracted from interp.c:50010 (function addNewMethodToCache). */

/*	Add the given entry to the method cache.
        The policy is as follows:
        Look for an empty entry anywhere in the reprobe chain.
        If found, install the new entry there.
        If not found, then install the new entry at the first probe position
        and delete the entries in the rest of the reprobe chain.
        This has two useful purposes:
        If there is active contention over the first slot, the second
        or third will likely be free for reentry after ejection.
        Also, flushing is good when reprobe chains are getting full. */

/* StackInterpreter>>#addNewMethodToCache: */

static NoDbgRegParms void addNewMethodToCache(sqInt classObj) {
  sqInt classTag;
  sqInt err;
  usqInt firstBytecode;
  sqInt hash;
  sqInt hashSqInt;
  sqInt methodHeader;
  sqInt p;
  sqInt primitiveIndex;
  usqInt probe;

  /* begin classTagForClass: */
  /* begin ensureBehaviorHash: */
  assert(addressCouldBeClassObj(classObj));

  /* eem 12/28/2021 the above asserft is too weak (and only an assert) */
  classTag = ((hashSqInt = (long32At((void *)(classObj + 4))) &
                           (identityHashHalfWordMask()))
                  ? hashSqInt
                  : (objCouldBeClassObj(classObj)
                         ? ((err = enterIntoClassTable(classObj))
                                ? -err
                                : (long32At((void *)(classObj + 4))) &
                                      (identityHashHalfWordMask()))
                         : -PrimErrBadReceiver));

  /* begin methodCacheHashOf:with: */
  hash = messageSelector ^ ((((usqInt)(classTag) << 2)));
  if (/* isOopCompiledMethod: */
      ((!(newMethod & (tagMask())))) &&
      (((byteAt((void *)(newMethod + (formatFieldByteOffset())))) &
        (formatMask())) >= (firstCompiledMethodFormat()))) {
    /* begin primitiveIndexOf: */
    /* begin methodHeaderOf: */
    assert(isCompiledMethod(newMethod));
    methodHeader =
        longAt((void *)((newMethod + BaseHeaderSize) +
                        ((((usqInt)(HeaderIndex) << (shiftForWord()))))));
    if (((methodHeader & AlternateHeaderHasPrimFlag) != 0)) {
      firstBytecode =
          (newMethod + ((LiteralStart + (((methodHeader >> 3)) &
                                         AlternateHeaderNumLiteralsMask)) *
                        BytesPerOop)) +
          BaseHeaderSize;
      primitiveIndex =
          (byteAt((void *)(firstBytecode + 1))) +
          ((((usqInt)((byteAt((void *)(firstBytecode + 2)))) << 8)));
    } else {
      primitiveIndex = 0;
    }

    /* begin functionPointerFor:inClass: */
    primitiveFunctionPointer =
        ((void (*)(void))(((((usqInt)primitiveIndex)) > MaxPrimitiveIndex
                               ? 0
                               : primitiveTable[primitiveIndex])));
  } else {
    assert(!((isNonImmediate(newMethod)) && (isForwarded(newMethod))));
    primitiveFunctionPointer = primitiveInvokeObjectAsMethod;
  }
  for (p = 0; p < CacheProbeMax; p += 1) {
    probe = (((usqInt)(hash)) >> p) & MethodCacheMask;
    if (!(methodCache[probe + MethodCacheSelector])) {
      methodCache[probe + MethodCacheSelector] = messageSelector;
      methodCache[probe + MethodCacheClass] = (classTagForClass(classObj));
      methodCache[probe + MethodCacheMethod] = newMethod;
      methodCache[probe + MethodCachePrimFunction] =
          (((sqIntptr_t)primitiveFunctionPointer));

      /* this for primitiveExternalMethod */
      lastMethodCacheProbeWrite = probe;
      return;
    }
  }

  /* OK, we failed to find an entry -- install at the first slot... */

  /* first probe */
  probe = hash & MethodCacheMask;
  methodCache[probe + MethodCacheSelector] = messageSelector;
  methodCache[probe + MethodCacheClass] = (classTagForClass(classObj));
  methodCache[probe + MethodCacheMethod] = newMethod;
  methodCache[probe + MethodCachePrimFunction] =
      (((sqIntptr_t)primitiveFunctionPointer));

  /* this for primitiveExternalMethod */
  /* ...and zap the following entries */
  lastMethodCacheProbeWrite = probe;
  for (p = 1; p < CacheProbeMax; p += 1) {
    probe = (((usqInt)(hash)) >> p) & MethodCacheMask;
    methodCache[probe + MethodCacheSelector] = 0;
  }
}