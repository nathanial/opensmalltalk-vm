/* Extracted from interp.c:36883 (function inPlaceBecomeandcopyHashFlag). */

/*	in an effort to fix a compiler bug with two-way become post r3427 */
/*	Do become in place by swapping object contents. */

/* SpurMemoryManager>>#inPlaceBecome:and:copyHashFlag: */

static NeverInline void inPlaceBecomeandcopyHashFlag(sqInt obj1, sqInt obj2,
                                                     sqInt copyHashFlag) {
  sqInt fmt;
  sqLong hashBits;
  sqLong header1;
  sqLong header2;
  sqInt i;
  sqInt o1HasYoung;
  sqInt o2HasYoung;
  sqLong remembered;
  sqInt temp1;
  sqInt temp2;
  sqInt toDoLimit;

  assert((numSlotsOf(obj1)) == (numSlotsOf(obj2)));
  assert(((rawHashBitsOf(obj1)) == 0) ||
         ((classOrNilAtIndex(rawHashBitsOf(obj1))) != obj1));
  assert(((rawHashBitsOf(obj2)) == 0) ||
         ((classOrNilAtIndex(rawHashBitsOf(obj2))) != obj2));

  /* swap headers, but swapping headers swaps remembered bits and hashes;
     remembered bits must be unswapped and hashes may be unswapped if
     copyHash is false. */

  /* begin cleverSwapHeaders:and:copyHashFlag: */
  header1 = long64At((void *)(obj1));
  header2 = long64At((void *)(obj2));
  remembered = (header1 ^ header2) & (1U << (rememberedBitShift()));
  if (remembered) {
    header1 = header1 ^ remembered;
    header2 = header2 ^ remembered;
  }

  /* swapping headers swaps hash; if not copyHashFlag then unswap hash */
  if (!copyHashFlag) {
    hashBits =
        (header1 ^ header2) & 0x3FFFFF00000000LL /* identityHashFullWordMask */;
    if (hashBits) {
      header1 = header1 ^ hashBits;
      header2 = header2 ^ hashBits;
    }
  }
  long64Atput((void *)(obj1), header2);
  long64Atput((void *)(obj2), header1);
  o1HasYoung = (o2HasYoung = 0);
  toDoLimit = ((numSlotsOf(obj1))) - 1;
  for (i = 0; i <= toDoLimit; i += 1) {
    temp1 = fetchPointerofObject(i, obj1);
    temp2 = fetchPointerofObject(i, obj2);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(obj1)) && (!(isForwarded(obj1))));
    assert(validStorePointerUncheckedArgs(i, obj1, temp2));
    longAtput((void *)((obj1 + BaseHeaderSize) +
                       ((((usqInt)(i) << (shiftForWord()))))),
              temp2);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(obj2)) && (!(isForwarded(obj2))));
    assert(validStorePointerUncheckedArgs(i, obj2, temp1));
    longAtput((void *)((obj2 + BaseHeaderSize) +
                       ((((usqInt)(i) << (shiftForWord()))))),
              temp1);
    if (/* isYoung: */
        ((!(temp2 & (tagMask())))) && (oopisLessThan(temp2, oldSpaceStart))) {
      o1HasYoung = 1;
    }
    if (/* isYoung: */
        ((!(temp1 & (tagMask())))) && (oopisLessThan(temp1, oldSpaceStart))) {
      o2HasYoung = 1;
    }
  }
  if (o1HasYoung && ((/* begin isOldObject: */
                      assert(isNonImmediate(obj1)),
                      oopisGreaterThanOrEqualTo(obj1, oldSpaceStart)))) {
    fmt = (byteAt((void *)(obj1 + (formatFieldByteOffset())))) & (formatMask());
    if (!(/* isPureBitsFormat: */
          (fmt >= (sixtyFourBitIndexableFormat())) &&
          (fmt < (firstCompiledMethodFormat())))) {
      /* begin possibleRootStoreInto: */
      if (!((byteAt((void *)(obj1 + (formatFieldByteOffset())))) &
            (1U << (rememberedBitByteShift())))) {
        remember(obj1);
      }
    }
  }
  if (o2HasYoung && ((/* begin isOldObject: */
                      assert(isNonImmediate(obj2)),
                      oopisGreaterThanOrEqualTo(obj2, oldSpaceStart)))) {
    fmt = (byteAt((void *)(obj2 + (formatFieldByteOffset())))) & (formatMask());
    if (!(/* isPureBitsFormat: */
          (fmt >= (sixtyFourBitIndexableFormat())) &&
          (fmt < (firstCompiledMethodFormat())))) {
      /* begin possibleRootStoreInto: */
      if (!((byteAt((void *)(obj2 + (formatFieldByteOffset())))) &
            (1U << (rememberedBitByteShift())))) {
        remember(obj2);
      }
    }
  }
}
