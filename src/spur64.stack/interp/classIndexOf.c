/* Extracted from interp.c:33274 (function classIndexOf). */

/*	Answer objOop's classIndex from the object header.
        A note on performance:
        This routine uses longAt:, which does a 32-bit load on the 32-bit
   system, and a 64-bit load on the 64-bit system. Since the only 64-bit
   implementation(s) is/are little-endian, and since all of
   UNUSED,isPinned,isRemembered,format,isImmutable and classIndex fit within the
   least significant 32-bits we could use long32At: to access these, in the hope
   that the 32-bit access will be quicker on 64-bits by virtue of fetching half
   the bits. But experiments show that doing this does not produce any increase;
   at least any signal is lost in the noise. */

/* SpurMemoryManager>>#classIndexOf: */

sqInt classIndexOf(sqInt objOop) {
  return (longAt((void *)(objOop))) & (classIndexMask());
}