/* Extracted from interp.c:12019 (function primitiveAsCharacter). */

	/* InterpreterPrimitives>>#primitiveAsCharacter */

static void
primitiveAsCharacter(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt characterCode;
    sqInt characterObject;
    char *sp;

	characterCode = longAt(GIV(stackPointer));
	if (!(((((characterCode) & 7) == 1))
		 && (((characterCode = (characterCode >> 3)),
		/* isInRangeCharacterCode: */
			((characterCode >= 0) && (characterCode <= (0x3FFFFFFF))))))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = (GIV(argumentCount)
					? PrimErrBadArgument
					: PrimErrBadReceiver);
		return;
	}
	characterObject = ((((usqInt)(characterCode) << (numTagBits())))) + (characterTag());

	/* begin pop:thenPush: */
	longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),characterObject);
	GIV(stackPointer) = sp;
}