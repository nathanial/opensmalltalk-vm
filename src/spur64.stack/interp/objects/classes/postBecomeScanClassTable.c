/* Extracted from interp.c:42461 (function postBecomeScanClassTable). */

/*	Scan the class table post-become (iff an active class object was
   becommed) to ensure no forwarding pointers, and no unhashed classes exist in
   the class table.

        Note that one-way become can cause duplications in the class table.
        So if pointer objects have been becommed, scan all pages in the class
        table and eliminate ay entries that have a zero hash.
        doBecome:and:copyHash: has
        already ensured that any becomed class has been stored at the right
   index. */

/* SpurMemoryManager>>#postBecomeScanClassTable: */

static void postBecomeScanClassTable(sqInt effectsFlags) {
  sqInt classOrNil;
  sqInt i;
  sqInt j;
  usqInt numSlots;
  sqInt page;
  sqInt referent;
  sqInt toDoLimit;

  assert(validClassTableRootPages());
  if (!(((effectsFlags & BecamePointerObjectFlag) != 0))) {
    return;
  }
  for (i = 0; i < numClassTablePages; i += 1) {
    page = fetchPointerofObject(i, hiddenRootsObj);
    assert(!(isForwarded(page)));
    toDoLimit =
        ((/* begin numSlotsOf: */
          assert((classIndexOf(page)) > (isForwardedObjectClassIndexPun())),
          (((numSlots =
                 byteAt((void *)(page + (numSlotsFieldByteOffset()))))) ==
                   (numSlotsMask())
               ? ((((usqInt)((
                     (sqInt)((usqInt)((longAt((void *)(page - BaseHeaderSize))))
                             << 8)))))) >>
                     8
               : numSlots))) -
        1;
    for (j = 0; j <= toDoLimit; j += 1) {
      classOrNil = fetchPointerofObject(j, page);
      if (classOrNil != nilObj) {
        if ((!((longAt((void *)(classOrNil))) &
               ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
          classOrNil = followForwarded(classOrNil);

          /* begin storePointer:ofObject:withValue: */
          assert(validStorePointerArgs(j, page, classOrNil));
          assert(isNonImmediate(page));
          if (oopisGreaterThanOrEqualTo(page, oldSpaceStart)) {
            if (/* isYoung: */
                ((!(classOrNil & (tagMask())))) &&
                (oopisLessThan(classOrNil, oldSpaceStart))) {
              /* begin possibleRootStoreInto: */
              if (!((byteAt((void *)(page + (formatFieldByteOffset())))) &
                    (1U << (rememberedBitByteShift())))) {
                remember(page);
              }
            }
          }

          /* most stores into young objects */
          longAtput((void *)((page + BaseHeaderSize) +
                             ((((usqInt)(j) << (shiftForWord()))))),
                    classOrNil);
        }
        if (!((long32At((void *)(classOrNil + 4))) &
              (identityHashHalfWordMask()))) {
          /* begin storePointerUnchecked:ofObject:withValue: */
          assert((isNonImmediate(page)) && (!(isForwarded(page))));
          assert(validStorePointerUncheckedArgs(j, page, nilObj));
          longAtput((void *)((page + BaseHeaderSize) +
                             ((((usqInt)(j) << (shiftForWord()))))),
                    nilObj);

          /* If the removed class is before the classTableIndex, set the
             classTableIndex to point to the empty slot so as to reuse it asap.
           */
          if ((((((usqInt)(i) << (classTableMajorIndexShift())))) + j) <
              classTableIndex) {
            classTableIndex =
                ((((usqInt)(i) << (classTableMajorIndexShift())))) + j;
          }
        }
      }
    }
  }

  /* classTableIndex must never index the first page, which is reserved for
   * classes known to the VM. */
  assert(classTableIndex >= (1U << (classTableMajorIndexShift())));
}