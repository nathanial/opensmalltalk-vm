/* Extracted from interp.c:35832 (function freeObject). */

/*	Free an object in oldSpace. Coalesce if possible to reduce fragmentation. */

	/* SpurMemoryManager>>#freeObject: */

sqInt
freeObject(sqInt objOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqInt bytes;
    usqInt chunkBytes;
    sqInt index;
    sqInt next;
    usqInt numSlots;
    usqInt start;

	assert(isInOldSpace(objOop));
	if ((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift()))) {
		/* begin forgetObject: */
		assert(GIV(rememberedSetSize) > 0);
		assert(isRemembered(objOop));
		setIsRememberedOfto(objOop, 0);
		if (!(objOop == (GIV(rememberedSet)[GIV(rememberedSetSize) - 1]))) {
			index = 0;
			while (index < GIV(rememberedSetSize)) {
				if (objOop == (GIV(rememberedSet)[index])) {
					GIV(rememberedSet)[index] = (GIV(rememberedSet)[GIV(rememberedSetSize) - 1]);
					index = GIV(rememberedSetSize);
				}
				else {
					index += 1;
				}
			}
		}
		GIV(rememberedSetSize) -= 1;
		assert(GIV(rememberedSetSize) >= 0);
	}
	bytes = bytesInBody(objOop);
	start = /* startOfObject: */
			((byteAt((void *)(objOop + (numSlotsFieldByteOffset())))) == (numSlotsMask())
				? objOop - BaseHeaderSize
				: objOop);

	/* begin objectStartingAt: */
	numSlots = byteAt((void *)((start + bytes) + (numSlotsFieldByteOffset())));
	next = (numSlots == (numSlotsMask())
				? (start + bytes) + BaseHeaderSize
				: start + bytes);
	if (((longAt((void *)(next))) & (classIndexMask())) == (isFreeObjectClassIndexPun())) {
		/* begin detachFreeObject: */
		chunkBytes = bytesInBody(next);
		GIV(totalFreeOldSpace) -= chunkBytes;
		unlinkFreeChunkchunkBytes(next, chunkBytes);
		bytes += bytesInBody(next);
	}
	GIV(totalFreeOldSpace) += bytes;
	return freeChunkWithBytesat(bytes, start);
}