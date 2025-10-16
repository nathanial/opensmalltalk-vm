/* Extracted from interp.c:47704 (function isMobile). */

static NoDbgRegParms sqInt
isMobile(sqInt obj)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return (oopisGreaterThanOrEqualToandLessThanOrEqualTo(obj, GIV(mobileStart), GIV(lastMobileObject)))
	 && (!(((byteAt((void *)(obj + (formatFieldByteOffset())))) & (1U << (pinnedBitByteShift()))) != 0));
}