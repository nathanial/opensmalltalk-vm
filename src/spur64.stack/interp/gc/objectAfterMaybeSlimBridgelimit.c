/* Extracted from interp.c:27602 (function objectAfterMaybeSlimBridgelimit). */

/*	Object parsing.
        1. all objects have at least a word following the header, for a
   forwarding pointer. 2. objects with an overflow size have a preceding word
   with a saturated numSlots. If the word following an object doesn't have a
   saturated numSlots field it must be a single-header object. If the word
   following does have a saturated numSlots it must be the overflow size word.

        This variation on objectAfter:limit: allows for a single (64-bit) word
        bridge which may be needed
        to bridge from an almost full pastSpace to eden. It is only used in the
        flat enumerators that use
        startAddressForBridgedHeapEnumeration and enumerate over pastSpace, eden
        and oldSpace
        in that order. Note that the order for allObjects, and allInstances
        enumerates over oldSpace first.

        This hack is cheap. It increases the size of the objectAfter code, but
        saves two extra copies of
        the inner loop, since the inner loop now enumerates over all of
   pastSpace, eden and oldSpace. The test for a slim bridge is only performed if
   applied to an overflow header, and typically only 1 in 400 objects have
   overflow headers in 32-bits, 1 in 500 in 64-bits. The complication is that
        image segment loading evaporates the word array by setting the overflow
        slots to 1, and this
        is ambiguous with a slimbridge. The resolution is that if the
   segmentArray has an overflow header, and is in new space, then its slot size
   can be zeroed and its overflow header changed to a slimbridge.

        At some point we should allow slimbridges (slivers?) throughout object
        memory, and use them to
        provide object alignment by slimbridges (slivers?) padding up to the
        following (aligned) object.
 */

/* Spur64BitMemoryManager>>#objectAfterMaybeSlimBridge:limit: */

static sqInt objectAfterMaybeSlimBridgelimit(sqInt objOop, sqInt limit) {
  sqInt followingWord;
  usqInt followingWordAddress;

  followingWordAddress = addressAfter(objOop);
  if (oopisGreaterThanOrEqualTo(followingWordAddress, limit)) {
    return limit;
  }
  followingWord = longAt((void *)(followingWordAddress));
  return ((((usqInt)(followingWord)) >> (numSlotsFullShift())) ==
                  (numSlotsMask())
              ? ((oopisLessThan(objOop, oldSpaceStart)) &&
                         ((followingWord & 0xFFFFFFFFFFFFFFLL) == 1)
                     ? (followingWordAddress + BaseHeaderSize) + BaseHeaderSize
                     : followingWordAddress + BaseHeaderSize)
              : followingWordAddress);
}