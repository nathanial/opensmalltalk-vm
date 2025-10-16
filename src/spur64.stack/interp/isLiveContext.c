/* Extracted from interp.c:55096 (function isLiveContext). */

static NoDbgRegParms sqInt
isLiveContext(sqInt oop)
{
	assert(!((isOopForwarded(oop))));
	if (!(/* isContext: */
			((!(oop & (tagMask()))))
		 && (((longAt((void *)(oop))) & (classIndexMask())) == ClassMethodContextCompactIndex))) {
		return 0;
	}
	if ((!((longAt((void *)((oop + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))))) & (tagMask())))) {
		return ((((longAt((void *)((oop + BaseHeaderSize) + ((((usqInt)(InstructionPointerIndex) << (shiftForWord())))))))) & 7) == 1);
	}
	return !(isWidowedContext(oop));
}