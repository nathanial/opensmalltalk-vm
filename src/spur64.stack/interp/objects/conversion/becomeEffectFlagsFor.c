/* Extracted from interp.c:31231 (function becomeEffectFlagsFor). */

/*	Answer the appropriate become effect flags for objOop, or 0 if none.
        The effect flags determine how much work is done after the become
        in following forwarding pointers, voiding method caches, etc. */

/* SpurMemoryManager>>#becomeEffectFlagsFor: */

static sqInt becomeEffectFlagsFor(sqInt objOop) {
  sqInt hash;

  hash = 0;
  if (((byteAt((void *)(objOop + (formatFieldByteOffset())))) &
       (formatMask())) <= 5 /* lastPointerFormat */) {
    if ((((hash = (long32At((void *)(objOop + 4))) &
                  (identityHashHalfWordMask()))) != 0) &&
        ((classAtIndex(hash)) == objOop)) {
      return BecamePointerObjectFlag + BecameActiveClassFlag;
    } else {
      return BecamePointerObjectFlag;
    }
  } else {
    return (((byteAt((void *)(objOop + (formatFieldByteOffset())))) &
             (formatMask())) >= (firstCompiledMethodFormat())
                ? BecameCompiledMethodFlag
                : 0);
  }
}