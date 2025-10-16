/* Extracted from interp.c:31231 (function becomeEffectFlagsFor). */

static NoDbgRegParms sqInt
becomeEffectFlagsFor(sqInt objOop)
{
    sqInt hash;

	hash = 0;
	if (((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask())) <= 5 /* lastPointerFormat */) {
		if ((((hash = (long32At((void *)(objOop + 4))) & (identityHashHalfWordMask()))) != 0)
		 && ((classAtIndex(hash)) == objOop)) {
			return BecamePointerObjectFlag + BecameActiveClassFlag;
		}
		else {
			return BecamePointerObjectFlag;
		}
	}
	else {
		return (((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask())) >= (firstCompiledMethodFormat())
				? BecameCompiledMethodFlag
				: 0);
	}
}