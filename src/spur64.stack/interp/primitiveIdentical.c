/* Extracted from interp.c:17734 (function primitiveIdentical). */

/*	is the receiver/first argument the same object as the (last) argument?.
	pop argumentCount because this can be used as a mirror primitive. */

	/* InterpreterPrimitives>>#primitiveIdentical */

static void
primitiveIdentical(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt otherObject;
    char *sp;
    sqInt thisObject;

	thisObject = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	otherObject = longAt(GIV(stackPointer));
	if ((/* isOopForwarded: */
		((!(otherObject & (tagMask()))))
	 && ((!((longAt((void *)(otherObject))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))))
	 || ((GIV(argumentCount) > 1)
	 && (/* isOopForwarded: */
		((!(thisObject & (tagMask()))))
	 && ((!((longAt((void *)(thisObject))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadArgument;
	}
	else {
		/* begin pop:thenPushBool: */
		longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),/* booleanObjectOf: */
			(thisObject == otherObject
				? GIV(trueObj)
				: GIV(falseObj)));
		GIV(stackPointer) = sp;
	}
}