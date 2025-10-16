/* Extracted from interp.c:61868 (function reapAndResetErrorCodeToheader). */

static NoDbgRegParms NeverInline void
reapAndResetErrorCodeToheader(char *theSP, sqInt methodHeader)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqInt initialPC;

	assert(GIV(primFailCode) != 0);
	initialPC = (((((usqInt)(pointerForOop(GIV(newMethod))))) + ((LiteralStart + ((/* begin literalCountOfMethodHeader: */
	assert((((methodHeader) & 7) == 1)),
/* literalCountOfAlternateHeader: */
	((methodHeader >> 3)) & AlternateHeaderNumLiteralsMask))) * BytesPerOop)) + BaseHeaderSize) + 3 /* sizeOfCallPrimitiveBytecode: */;
	if ((byteAt((void *)(initialPC))) == (/* longStoreBytecodeForHeader: */
		((((sqLong) methodHeader)) < 0
			? AltLongStoreBytecode
			: LongStoreBytecode))) {
		longAtput(theSP,getErrorObjectFromPrimFailCode());
	}
	GIV(primFailCode) = 0;
}