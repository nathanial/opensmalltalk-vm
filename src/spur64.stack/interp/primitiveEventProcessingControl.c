/* Extracted from interp.c:58469 (function primitiveEventProcessingControl). */

/*	With 0 args answers whether ioProcessEvents is enabled and being called.
	With 1 arg expects a boolean which will enable ioProcessEvents if true and
	disable it if false, answering its previous state. */

	/* StackInterpreter>>#primitiveEventProcessingControl */

EXPORT(void)
primitiveEventProcessingControl(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    int enabled;
    char *sp;

	enabled = inIOProcessEvents >= 0;
	if (!GIV(argumentCount)) {
		/* begin pop:thenPushBool: */
		longAtput((sp = GIV(stackPointer)),/* booleanObjectOf: */
			(enabled
				? GIV(trueObj)
				: GIV(falseObj)));
		GIV(stackPointer) = sp;
		return;
	}
	if (GIV(argumentCount) == 1) {
		if ((longAt(GIV(stackPointer))) == GIV(trueObj)) {
			if (inIOProcessEvents < 0) {
				inIOProcessEvents = 0;
			}
		}
		else {
			if ((longAt(GIV(stackPointer))) == GIV(falseObj)) {
				inIOProcessEvents = -1;
			}
			else {
				/* primitiveFailFor: */
				GIV(primFailCode) = PrimErrBadArgument;
				return;
			}
		}

		/* begin pop:thenPushBool: */
		longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),/* booleanObjectOf: */
			(enabled
				? GIV(trueObj)
				: GIV(falseObj)));
		GIV(stackPointer) = sp;
		return;
	}

	/* primitiveFailFor: */
	GIV(primFailCode) = PrimErrBadNumArgs;
}