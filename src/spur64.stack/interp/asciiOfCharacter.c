/* Extracted from interp.c:50165 (function asciiOfCharacter). */

/*	Returns an integer object */

	/* StackInterpreter>>#asciiOfCharacter: */

static NoDbgRegParms sqInt
asciiOfCharacter(sqInt characterObj)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	if (((characterObj & (characterTag())) != 0)) {
		return characterObj - ((characterTag()) - (smallIntegerTag()));
	}

	/* primitiveFailFor: */
	GIV(primFailCode) = PrimErrBadArgument;
	return ConstZero;
}