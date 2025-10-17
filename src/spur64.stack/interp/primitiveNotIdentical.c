/* Extracted from interp.c:19899 (function primitiveNotIdentical). */

/*	is the receiver/first argument not the same object as the (last)
	argument?. pop argumentCount because this can be used as a mirror
	primitive. 
 */
/*	is the receiver/first argument the same object as the (last) argument?.
	pop argumentCount because this can be used as a mirror primitive. */

	/* InterpreterPrimitives>>#primitiveNotIdentical */

static void
primitiveNotIdentical(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt otherObject;
    char *sp;
    sqInt thisObject;

	thisObject = longAt(stackPointer + (1 * BytesPerWord));
	otherObject = longAt(stackPointer);
	if ((/* isOopForwarded: */
		((!(otherObject & (tagMask()))))
	 && ((!((longAt((void *)(otherObject))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))))
	 || ((argumentCount > 1)
	 && (/* isOopForwarded: */
		((!(thisObject & (tagMask()))))
	 && ((!((longAt((void *)(thisObject))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))))) {
		/* primitiveFailFor: */
		primFailCode = PrimErrBadArgument;
	}
	else {
		/* begin pop:thenPushBool: */
		longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),/* booleanObjectOf: */
			(thisObject != otherObject
				? trueObj
				: falseObj));
		stackPointer = sp;
	}
}