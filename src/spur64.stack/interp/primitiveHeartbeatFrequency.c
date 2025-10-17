/* Extracted from interp.c:17625 (function primitiveHeartbeatFrequency). */

/*	Answer the heartbeat frequency in beats per second. If the argument is
	true, reset the frequency measure. */

	/* InterpreterPrimitives>>#primitiveHeartbeatFrequency */

EXPORT(sqInt)
primitiveHeartbeatFrequency(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    unsigned int integerValue;
    sqInt oop;
    sqInt reset;
    char *sp;

	reset = (GIV(argumentCount) == 1)
		 && ((longAt(GIV(stackPointer))) == GIV(trueObj));
	integerValue = ((unsigned int) (ioHeartbeatFrequency(reset)));

	/* begin positive32BitIntegerFor: */
	oop = ((((((usqInt)integerValue)) & 0xFFFFFFFFU) << 3) | 1);
	longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),oop);
	GIV(stackPointer) = sp;
	return 0;
}