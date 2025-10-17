/* Extracted from interp.c:49310 (function prepareForSnapshot). */

/*	shorten all segments by any trailing free space. */

/* SpurSegmentManager>>#prepareForSnapshot */

static NeverInline void prepareForSnapshot(void) {
  sqInt cameFrom;
  SpurSegmentInfo *cascade0;
  usqInt chunkBytes;
  sqInt followingWord;
  usqInt followingWordAddress;
  usqInt freeChunk;
  sqInt i;
  sqInt largeChild;
  sqInt newEndOfMemory;
  sqInt next;
  sqInt node;
  SpurSegmentInfo *seg;
  SpurSegmentInfo *selfOfSegSize;
  sqInt smallChild;
  sqInt treeNode;

  checkSegments();
  for (i = 0; i < numSegments; i += 1) {
    cascade0 = (&(segments[i]));
    (cascade0->savedSegSize = ((segments[i]).segSize));
    (cascade0->lastFreeObject = null);
  }

  /* Ideally finding the lastFreeObject of each segment would be
     done in some linear pass through the heap.  But for now KISS. */

  /* begin freeTreeNodesDo: */
  treeNode = freeLists[0];
  if (!treeNode) {
    goto l2;
  }
  cameFrom = -1;
  do {
    assert((bytesInBody(treeNode)) >= ((numFreeLists()) * (allocationUnit())));
    smallChild = fetchPointerofObject(3U, treeNode);
    largeChild = fetchPointerofObject(4U, treeNode);
    assert((smallChild == 0) ||
           (treeNode ==
            (fetchPointerofFreeChunk(freeChunkParentIndex(), smallChild))));
    assert((largeChild == 0) ||
           (treeNode ==
            (fetchPointerofFreeChunk(freeChunkParentIndex(), largeChild))));

    /* apply if the node has no children, or it has no large children and we're
       returning from the small child, or we're returning from the large child.
     */
    if (((smallChild == 0) && (largeChild == 0)) ||
        ((largeChild ? cameFrom == largeChild : cameFrom == smallChild))) {
      node = treeNode;
      while (node != 0) {
        /* begin objectAfter:limit: */
        followingWordAddress = addressAfter(node);
        if (oopisGreaterThanOrEqualTo(followingWordAddress, endOfMemory)) {
          next = endOfMemory;
          goto l1;
        }
        followingWord = longAt((void *)(followingWordAddress));
        next = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) ==
                        (numSlotsMask())
                    ? followingWordAddress + BaseHeaderSize
                    : followingWordAddress);
        /* end objectAfter:limit: */
      l1:
        if (((longAt((void *)(next))) & (classIndexMask())) ==
            (segmentBridgePun())) {
          seg = segmentContainingObj(node);
          (seg->lastFreeObject = node);
          node = 0;
        } else {
          node = fetchPointerofObject(0U, node);
        }
      }

      /* and since we've applied we must move on up */
      cameFrom = treeNode;
      treeNode = fetchPointerofObject(2U, treeNode);
    } else {
      if ((smallChild != 0) && (cameFrom != smallChild)) {
        treeNode = smallChild;
      } else {
        assert(largeChild != 0);
        treeNode = largeChild;
      }
      cameFrom = -1;
    }
  } while (treeNode != 0);
  /* end freeTreeNodesDo: */
l2:
  for (i = 0; i < numSegments; i += 1) {
    if ((freeChunk = ((segments[i]).lastFreeObject))) {
      /* begin detachFreeObject: */
      chunkBytes = bytesInBody(freeChunk);
      totalFreeOldSpace -= chunkBytes;
      unlinkFreeChunkchunkBytes(freeChunk, chunkBytes);
      selfOfSegSize = (&(segments[i]));
      (selfOfSegSize->segSize =
           ((/* startOfObject: */
             ((byteAt((void *)(freeChunk + (numSlotsFieldByteOffset())))) ==
                      (numSlotsMask())
                  ? freeChunk - BaseHeaderSize
                  : freeChunk)) +
            (2 * BaseHeaderSize)) -
           (((segments[i]).segStart)));
      bridgeFromto((&(segments[i])),
                   (i < (numSegments - 1) ? (&(segments[i + 1])) : 0));
    }
  }

  /* perhaps this should read
     manager setEndOfMemory: 0; assimilateNewSegment: (segments at: numSegments
     - 1) */
  newEndOfMemory = ((((segments[numSegments - 1]).segSize)) +
                    (((segments[numSegments - 1]).segStart))) -
                   (2 * BaseHeaderSize);

  /* begin setEndOfMemory: */
  endOfMemory = newEndOfMemory;
  if (freeOldSpaceStart > newEndOfMemory) {
    freeOldSpaceStart = newEndOfMemory;
  }
}