/* Extracted from interp.c:45693 (function startOfObject). */

/*	Answer the start of objOop, which is either the address of the overflow
        size word, or objOop itself, depending on the size of the object. This
   may be applied to any kind of object, normal, forwarders or free chunks. */

/* SpurMemoryManager>>#startOfObject: */

static NoDbgRegParms usqInt startOfObject(sqInt objOop) {
  return ((byteAt((void *)(objOop + (numSlotsFieldByteOffset())))) ==
                  (numSlotsMask())
              ? objOop - BaseHeaderSize
              : objOop);
}