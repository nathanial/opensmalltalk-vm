/* Extracted from interp.c:45530 (function setIsRememberedOfto). */

static NoDbgRegParms void
setIsRememberedOfto(sqInt objOop, sqInt aBoolean)
{
	assert(!((isFreeObject(objOop))));
	byteAtput((void *)(objOop + (formatFieldByteOffset())),(aBoolean
			? (byteAt((void *)(objOop + (formatFieldByteOffset())))) | (1U << (rememberedBitByteShift()))
			: (byteAt((void *)(objOop + (formatFieldByteOffset())))) & (0xFF - (1U << (rememberedBitByteShift())))));
}