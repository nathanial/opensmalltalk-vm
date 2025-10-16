/* Extracted from interp.c:19899 (function primitiveNotIdentical). */

static void
primitiveNotIdentical(void)
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
			(thisObject != otherObject
				? GIV(trueObj)
				: GIV(falseObj)));
		GIV(stackPointer) = sp;
	}
}