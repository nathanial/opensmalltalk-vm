/* Extracted from interp.c:26446 (function
 * allocateSlotsForPinningInOldSpacebytesformatclassIndex). */

/*	Answer the oop of a chunk of space in oldSpace with numSlots slots. Try
        and allocate in a segment that already includes pinned objects. The
   header of the result will have been filled-in but not the contents. */

/* Spur64BitMemoryManager>>#allocateSlotsForPinningInOldSpace:bytes:format:classIndex:
 */

static sqInt allocateSlotsForPinningInOldSpacebytesformatclassIndex(
    sqInt numSlots, usqInt totalBytes, sqInt formatField, sqInt classIndex) {
  sqInt acceptedChunk;
  sqInt acceptedNode;
  sqInt child;
  usqInt childBytes;
  sqInt chunk;
  usqInt index;
  usqInt initialIndex;
  sqInt lilliputian;
  sqInt lilliputianSqInt;
  sqInt next;
  sqInt nextFreeChunk;
  sqInt nextSqInt;
  sqInt node;
  sqInt prev;

  /* begin allocateOldSpaceChunkOfBytes:suchThat: */
  next = 0;
  prev = 0;

  /* be optimistic (& don't wait for the write) */
  totalFreeOldSpace -= totalBytes;
  initialIndex = totalBytes / 8 /* allocationUnit */;
  if ((initialIndex < 64 /* numFreeLists */) &&
      ((1ULL << initialIndex) <= freeListsMask)) {
    if (((freeListsMask & (1ULL << initialIndex)) != 0)) {
      if ((node = freeLists[initialIndex])) {
        prev = 0;
        while (node != 0) {
          assert(node == (startOfObject(node)));

          /* begin assertValidFreeObject: */
          assert(assertInnerValidFreeObject(node));
          next = longAt(
              (void *)((node + BaseHeaderSize) + (0U << (shiftForWord()))));
          if (((segmentContainingObj(node))->containsPinned)) {
            if (prev) {
              /* begin setNextFreeChunkOf:withValue:chunkBytes: */
              /* begin isLilliputianSize: */
              assert(totalBytes >= (BaseHeaderSize + (allocationUnit())));
              lilliputianSqInt =
                  totalBytes == (BaseHeaderSize + 8 /* allocationUnit */);

              /* begin setNextFreeChunkOf:withValue:isLilliputianSize: */
              /* begin storePointer:ofFreeChunk:withValue: */
              assert(isFreeObject(prev));
              assert((next == 0) || (isFreeObject(next)));
              longAtput(
                  (void *)((prev + BaseHeaderSize) + (0U << (shiftForWord()))),
                  next);
              if ((next != 0) && (!lilliputianSqInt)) {
                /* begin storePointer:ofFreeChunk:withValue: */
                assert(isFreeObject(next));
                assert((prev == 0) || (isFreeObject(prev)));
                longAtput((void *)((next + BaseHeaderSize) +
                                   (1U << (shiftForWord()))),
                          prev);
              }
            } else {
              /* begin unlinkFreeChunk:atIndex:chunkBytes: */
              /* begin isLilliputianSize: */
              assert(totalBytes >= (BaseHeaderSize + (allocationUnit())));
              lilliputian =
                  totalBytes == (BaseHeaderSize + 8 /* allocationUnit */);

              /* begin unlinkFreeChunk:atIndex:isLilliputianSize: */
              assert(((bytesInBody(node)) ==
                      (initialIndex * (allocationUnit()))) &&
                     ((initialIndex > 1) && ((startOfObject(node)) == node)));

              /* For some reason the assertion is not compiled correctly */
              freeLists[initialIndex] =
                  ((nextSqInt = longAt((void *)((node + BaseHeaderSize) +
                                                (0U << (shiftForWord()))))));
              if ((!lilliputian) && (nextSqInt != 0)) {
                /* begin storePointer:ofFreeChunk:withValue: */
                assert(isFreeObject(nextSqInt));
                longAtput((void *)((nextSqInt + BaseHeaderSize) +
                                   (1U << (shiftForWord()))),
                          0);
              }
            }
            chunk = node;
            goto l1;
          }
          prev = node;
          node = next;
        }
      } else {
        freeListsMask -= 1ULL << initialIndex;
      }
    }

    /* first search for free chunks of a multiple of chunkBytes in size */
    index = initialIndex;
    while ((((index += initialIndex)) < 64 /* numFreeLists */) &&
           ((1ULL << index) <= freeListsMask)) {
      if (((freeListsMask & (1ULL << index)) != 0)) {
        if ((node = freeLists[index])) {
          prev = 0;
          while (node != 0) {
            assert(node == (startOfObject(node)));

            /* begin assertValidFreeObject: */
            assert(assertInnerValidFreeObject(node));
            next = longAt(
                (void *)((node + BaseHeaderSize) + (0U << (shiftForWord()))));
            if (((segmentContainingObj(node))->containsPinned)) {
              if (prev) {
                /* begin setNextFreeChunkOf:withValue:isLilliputianSize: */
                /* begin storePointer:ofFreeChunk:withValue: */
                assert(isFreeObject(prev));
                assert((next == 0) || (isFreeObject(next)));
                longAtput((void *)((prev + BaseHeaderSize) +
                                   (0U << (shiftForWord()))),
                          next);
                if (next) {
                  /* begin storePointer:ofFreeChunk:withValue: */
                  assert(isFreeObject(next));
                  assert((prev == 0) || (isFreeObject(prev)));
                  longAtput((void *)((next + BaseHeaderSize) +
                                     (1U << (shiftForWord()))),
                            prev);
                }
              } else {
                /* begin unlinkFreeChunk:atIndex:isLilliputianSize: */
                assert(((bytesInBody(node)) == (index * (allocationUnit()))) &&
                       ((index > 1) && ((startOfObject(node)) == node)));

                /* For some reason the assertion is not compiled correctly */
                freeLists[index] =
                    ((nextSqInt = longAt((void *)((node + BaseHeaderSize) +
                                                  (0U << (shiftForWord()))))));
                if (nextSqInt) {
                  /* begin storePointer:ofFreeChunk:withValue: */
                  assert(isFreeObject(nextSqInt));
                  longAtput((void *)((nextSqInt + BaseHeaderSize) +
                                     (1U << (shiftForWord()))),
                            0);
                }
              }
              freeChunkWithBytesat(
                  (index * 8 /* allocationUnit */) - totalBytes,
                  (/* startOfObject: */
                   ((byteAt((void *)(node + (numSlotsFieldByteOffset())))) ==
                            (numSlotsMask())
                        ? node - BaseHeaderSize
                        : node)) +
                      totalBytes);
              chunk = node;
              goto l1;
            }
            prev = node;
            node = next;
          }
        } else {
          freeListsMask -= 1ULL << index;
        }
      }
    }

    /* now get desperate and use the first that'll fit.
       Note that because the minimum free size is 16 bytes (2 * allocationUnit),
       to leave room for the forwarding pointer/next free link, we can only
       break chunks that are at least 16 bytes larger, hence start at
       initialIndex + 2. */
    index = initialIndex + 1;
    while ((((index += 1)) < 64 /* numFreeLists */) &&
           ((1ULL << index) <= freeListsMask)) {
      if (((freeListsMask & (1ULL << index)) != 0)) {
        if ((node = freeLists[index])) {
          prev = 0;
          while (node != 0) {
            assert(node == (startOfObject(node)));

            /* begin assertValidFreeObject: */
            assert(assertInnerValidFreeObject(node));
            next = longAt(
                (void *)((node + BaseHeaderSize) + (0U << (shiftForWord()))));
            if (((segmentContainingObj(node))->containsPinned)) {
              if (prev) {
                /* begin setNextFreeChunkOf:withValue:isLilliputianSize: */
                /* begin storePointer:ofFreeChunk:withValue: */
                assert(isFreeObject(prev));
                assert((next == 0) || (isFreeObject(next)));
                longAtput((void *)((prev + BaseHeaderSize) +
                                   (0U << (shiftForWord()))),
                          next);
                if (next) {
                  /* begin storePointer:ofFreeChunk:withValue: */
                  assert(isFreeObject(next));
                  assert((prev == 0) || (isFreeObject(prev)));
                  longAtput((void *)((next + BaseHeaderSize) +
                                     (1U << (shiftForWord()))),
                            prev);
                }
              } else {
                /* begin unlinkFreeChunk:atIndex:isLilliputianSize: */
                assert(((bytesInBody(node)) == (index * (allocationUnit()))) &&
                       ((index > 1) && ((startOfObject(node)) == node)));

                /* For some reason the assertion is not compiled correctly */
                freeLists[index] =
                    ((nextSqInt = longAt((void *)((node + BaseHeaderSize) +
                                                  (0U << (shiftForWord()))))));
                if (nextSqInt) {
                  /* begin storePointer:ofFreeChunk:withValue: */
                  assert(isFreeObject(nextSqInt));
                  longAtput((void *)((nextSqInt + BaseHeaderSize) +
                                     (1U << (shiftForWord()))),
                            0);
                }
              }
              freeChunkWithBytesat(
                  (index * 8 /* allocationUnit */) - totalBytes,
                  (/* startOfObject: */
                   ((byteAt((void *)(node + (numSlotsFieldByteOffset())))) ==
                            (numSlotsMask())
                        ? node - BaseHeaderSize
                        : node)) +
                      totalBytes);
              chunk = node;
              goto l1;
            }
            prev = node;
            node = next;
          }
        } else {
          freeListsMask -= 1ULL << index;
        }
      }
    }
  }

  /* Large chunk, or no space on small free lists.  Search the large chunk list.
     Large chunk list organized as a tree, each node of which is a list of
     chunks of the same size. Beneath the node are smaller and larger blocks.
     When the search ends parent should hold the smallest chunk at least as
     large as chunkBytes, or 0 if none.  acceptedChunk and acceptedNode save
     us from having to back-up when the acceptanceBlock filters-out all nodes
     of the right size, but there are nodes of the wrong size it does accept. */
  child = freeLists[0];
  node = (acceptedChunk = (acceptedNode = 0));
  while (child != 0) {
    /* begin assertValidFreeObject: */
    assert(assertInnerValidFreeObject(child));
    childBytes = bytesInBody(child);
    if (childBytes == totalBytes) {
      node = child;
      while (1) {
        prev = node;
        node = longAt(
            (void *)((node + BaseHeaderSize) + (0U << (shiftForWord()))));
        if (!(node != 0))
          break;
        if (((segmentContainingObj(node))->containsPinned)) {
          /* begin assertValidFreeObject: */
          assert(assertInnerValidFreeObject(node));
          nextFreeChunk = longAt(
              (void *)((node + BaseHeaderSize) + (0U << (shiftForWord()))));

          /* begin setNextFreeChunkOf:withValue:isLilliputianSize: */
          /* begin storePointer:ofFreeChunk:withValue: */
          assert(isFreeObject(prev));
          assert((nextFreeChunk == 0) || (isFreeObject(nextFreeChunk)));
          longAtput(
              (void *)((prev + BaseHeaderSize) + (0U << (shiftForWord()))),
              nextFreeChunk);
          if (nextFreeChunk) {
            /* begin storePointer:ofFreeChunk:withValue: */
            assert(isFreeObject(nextFreeChunk));
            assert((prev == 0) || (isFreeObject(prev)));
            longAtput((void *)((nextFreeChunk + BaseHeaderSize) +
                               (1U << (shiftForWord()))),
                      prev);
          }
          chunk = /* startOfObject: */
              ((byteAt((void *)(node + (numSlotsFieldByteOffset())))) ==
                       (numSlotsMask())
                   ? node - BaseHeaderSize
                   : node);
          goto l1;
        }
      }
      if (((segmentContainingObj(child))->containsPinned)) {
        next = longAt(
            (void *)((child + BaseHeaderSize) + (0U << (shiftForWord()))));
        if (next) {
          inFreeTreeReplacewith(child, next);
        } else {
          unlinkSolitaryFreeTreeNode(child);
        }

        /* no list; remove the interior node
           list; replace node with it */
        chunk = /* startOfObject: */
            ((byteAt((void *)(child + (numSlotsFieldByteOffset())))) ==
                     (numSlotsMask())
                 ? child - BaseHeaderSize
                 : child);
        goto l1;
      }
    }

    /* size match; try to remove from list at node. */
    if (child) {
      if (childBytes <= (totalBytes + 8 /* allocationUnit */)) {
        child = longAt(
            (void *)((child + BaseHeaderSize) + (4U << (shiftForWord()))));
      } else {
        node = child;
        child = longAt(
            (void *)((node + BaseHeaderSize) + (3U << (shiftForWord()))));
        if (!acceptedNode) {
          acceptedChunk = node;

          /* first search the list. */
          do {
            acceptedChunk = longAt((void *)((acceptedChunk + BaseHeaderSize) +
                                            (0U << (shiftForWord()))));
            if ((acceptedChunk != 0) &&
                (((segmentContainingObj(acceptedChunk))->containsPinned))) {
              acceptedNode = node;
            }
          } while ((acceptedChunk != 0) && (acceptedNode == 0));

          /* nothing on the list; will the node do?  This prefers
             acceptable nodes higher up the tree over acceptable
             list elements further down, but we haven't got all day... */
          if ((acceptedNode == 0) &&
              (((segmentContainingObj(node))->containsPinned))) {
            acceptedNode = node;

            /* break out of loop now we have an acceptedNode */
            child = 0;
          }
        }
      }
    }
  }
  if (acceptedNode) {
    if (acceptedChunk) {
      assert((bytesInBody(acceptedChunk)) >= (totalBytes + (allocationUnit())));
      while (1) {
        next = longAt((void *)((acceptedNode + BaseHeaderSize) +
                               (0U << (shiftForWord()))));
        if (!(next != acceptedChunk))
          break;
        acceptedNode = next;
      }
      nextFreeChunk = longAt((void *)((acceptedChunk + BaseHeaderSize) +
                                      (0U << (shiftForWord()))));

      /* begin setNextFreeChunkOf:withValue:isLilliputianSize: */
      /* begin storePointer:ofFreeChunk:withValue: */
      assert(isFreeObject(acceptedNode));
      assert((nextFreeChunk == 0) || (isFreeObject(nextFreeChunk)));
      longAtput(
          (void *)((acceptedNode + BaseHeaderSize) + (0U << (shiftForWord()))),
          nextFreeChunk);
      if (nextFreeChunk) {
        /* begin storePointer:ofFreeChunk:withValue: */
        assert(isFreeObject(nextFreeChunk));
        assert((acceptedNode == 0) || (isFreeObject(acceptedNode)));
        longAtput((void *)((nextFreeChunk + BaseHeaderSize) +
                           (1U << (shiftForWord()))),
                  acceptedNode);
      }
      freeChunkWithBytesat(
          (bytesInBody(acceptedChunk)) - totalBytes,
          (/* startOfObject: */
           ((byteAt((void *)(acceptedChunk + (numSlotsFieldByteOffset())))) ==
                    (numSlotsMask())
                ? acceptedChunk - BaseHeaderSize
                : acceptedChunk)) +
              totalBytes);
      chunk = /* startOfObject: */
          ((byteAt((void *)(acceptedChunk + (numSlotsFieldByteOffset())))) ==
                   (numSlotsMask())
               ? acceptedChunk - BaseHeaderSize
               : acceptedChunk);
      goto l1;
    }
    next = longAt(
        (void *)((acceptedNode + BaseHeaderSize) + (0U << (shiftForWord()))));
    if (next) {
      inFreeTreeReplacewith(acceptedNode, next);
    } else {
      unlinkSolitaryFreeTreeNode(acceptedNode);
    }

    /* no list; remove the interior node
       list; replace node with it */
    assert((bytesInBody(acceptedNode)) >= (totalBytes + (allocationUnit())));
    freeChunkWithBytesat(
        (bytesInBody(acceptedNode)) - totalBytes,
        (/* startOfObject: */
         ((byteAt((void *)(acceptedNode + (numSlotsFieldByteOffset())))) ==
                  (numSlotsMask())
              ? acceptedNode - BaseHeaderSize
              : acceptedNode)) +
            totalBytes);
    chunk = /* startOfObject: */
        ((byteAt((void *)(acceptedNode + (numSlotsFieldByteOffset())))) ==
                 (numSlotsMask())
             ? acceptedNode - BaseHeaderSize
             : acceptedNode);
    goto l1;
  }

  /* optimism was unfounded */
  totalFreeOldSpace += totalBytes;
  chunk = null;
  /* end allocateOldSpaceChunkOfBytes:suchThat: */
l1:
  if (!chunk) {
    chunk = allocateOldSpaceChunkOfBytes(totalBytes);
    if (!chunk) {
      return null;
    }
    ((segmentContainingObj(chunk))->containsPinned = 1);
  }
  if (numSlots >= (numSlotsMask())) {
    longAtput((void *)(chunk), numSlots + ((((usqInt)((numSlotsMask()))
                                             << (numSlotsFullShift())))));
    longAtput((void *)(chunk + BaseHeaderSize),
              (((((((usqLong)(numSlotsMask()))) << (numSlotsFullShift()))) +
                ((((usqInt)(formatField) << (formatShift()))))) +
               classIndex) |
                  (1U << (pinnedBitShift())));

    /* begin checkFreeSpace: */
    assert(bitsSetInFreeSpaceMaskForAllFreeLists());
    assert(totalFreeOldSpace == (totalFreeListBytes()));
    if (((checkForLeaks & (GCCheckFreeSpace | GCModeNewSpace)) ==
         (GCCheckFreeSpace | GCModeNewSpace))) {
      runLeakCheckerForFreeSpaceignoring(GCCheckFreeSpace, null);
    }
    return chunk + BaseHeaderSize;
  }

  /* for header parsing we put a saturated slot count in the prepended overflow
   * size word */
  longAtput((void *)(chunk),
            (((((((usqLong)numSlots)) << (numSlotsFullShift()))) +
              ((((usqInt)(formatField) << (formatShift()))))) +
             classIndex) |
                (1U << (pinnedBitShift())));

  /* begin checkFreeSpace: */
  assert(bitsSetInFreeSpaceMaskForAllFreeLists());
  assert(totalFreeOldSpace == (totalFreeListBytes()));
  if (((checkForLeaks & (GCCheckFreeSpace | GCModeNewSpace)) ==
       (GCCheckFreeSpace | GCModeNewSpace))) {
    runLeakCheckerForFreeSpaceignoring(GCCheckFreeSpace, null);
  }
  return chunk;
}