/* Extracted from interp.c:54716 (function homeMethodOf). */

sqInt
homeMethodOf(sqInt aMethodOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt lastLiteral;
    sqInt methodChain;

	assert(isOopCompiledMethod(aMethodOop));
	methodChain = aMethodOop;
	while (1) {
		lastLiteral = ultimateLiteralOf(methodChain);
		if (!(/* isOopCompiledMethod: */
				((!(lastLiteral & (tagMask()))))
			 && (((byteAt((void *)(lastLiteral + (formatFieldByteOffset())))) & (formatMask())) >= (firstCompiledMethodFormat())))) {
			return methodChain;
		}
		if (!(/* isOopCompiledMethod: */
				((!(lastLiteral & (tagMask()))))
			 && (((byteAt((void *)(lastLiteral + (formatFieldByteOffset())))) & (formatMask())) >= (firstCompiledMethodFormat())))) {
			return GIV(nilObj);
		}
		methodChain = lastLiteral;
	}
	return 0;
}