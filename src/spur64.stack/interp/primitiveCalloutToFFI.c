/* Extracted from interp.c:13329 (function primitiveCalloutToFFI). */

static void
primitiveCalloutToFFI(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    void (*primitiveCallout)(void);

	/* begin functionForPrimitiveCallout */
	if ((((sqInt)primitiveCalloutPointer)) == -1) {
		/* begin reloadPrimitiveCalloutPointer */
		primitiveCalloutPointer = ioLoadFunctionFrom("primitiveCallout", "SqueakFFIPrims");
	}
	primitiveCallout = ((void (*)(void)) primitiveCalloutPointer);
	if (primitiveCallout) {
		dispatchFunctionPointer(primitiveCallout);
	}
	else {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
	}
}