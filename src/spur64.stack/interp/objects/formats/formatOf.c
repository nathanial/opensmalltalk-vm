/* Extracted from interp.c:35749 (function formatOf). */

/*	0 = 0 sized objects (UndefinedObject True False et al)
        1 = non-indexable objects with inst vars (Point et al)
        2 = indexable objects with no inst vars (Array et al)
        3 = indexable objects with inst vars (MethodContext
   AdditionalMethodState et al) 4 = weak indexable objects with inst vars
   (WeakArray et al) 5 = weak non-indexable objects with inst vars (ephemerons)
   (Ephemeron) 6 unused, reserved for exotic pointer objects? 7 Forwarded
   Object, 1st field is pointer, rest of fields are ignored 8 unused, reserved
   for exotic non-pointer objects? 9 64-bit indexable 10 - 11 32-bit indexable
   (lsb = # of unused 32-bit fields, 11 unused in 32 bits) 12 - 15 16-bit
   indexable	(ls2b = # of unused 16-bit fields, 14 & 15 unused in 32-bits) 16
   - 23 byte indexable	(ls3b = # of unused 8-bit fields, 20, 21, 22, 23 unused
   in 32-bits) 24 - 31 compiled method	(ls3b = # of unused 8-bit fields, 20,
   21, 22, 23 unused in 32-bits) */
/*	A note on performance. Since the format field is, by design, aligned on
   a byte boundary in the fourth byte of the header (see
   headerForSlots:format:classIndex:) it is accessed as below. But it used to be
   accessed via
        ^(self longAt: objOop) >> self formatShift bitAnd: self formatMask
        This potentially involves more operations (a shift) and larger literals,
        In practice clang
        optimizes the long form to the byte form on x86_64, showing it is to be
        preferred on at
        least one architecture. On arm64 for example both forms take two
        instructions.  */

/* SpurMemoryManager>>#formatOf: */

static sqInt formatOf(sqInt objOop) {
  return (byteAt((void *)(objOop + (formatFieldByteOffset())))) &
         (formatMask());
}