/* Extracted from interp.c:50165 (function asciiOfCharacter). */

/*	Returns an integer object */

	/* StackInterpreter>>#asciiOfCharacter: */

static NoDbgRegParms sqInt
asciiOfCharacter(sqInt characterObj)
{
	if (((characterObj & (characterTag())) != 0)) {
		return characterObj - ((characterTag()) - (smallIntegerTag()));
	}

	/* primitiveFailFor: */
	primFailCode = PrimErrBadArgument;
	return ConstZero;
}