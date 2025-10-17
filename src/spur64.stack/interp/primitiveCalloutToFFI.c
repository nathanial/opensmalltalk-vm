/* Extracted from interp.c:13329 (function primitiveCalloutToFFI). */

/*	Perform a function call to a foreign function.
	Only invoked from method containing explicit external call spec.
	Due to this we use the pluggable prim mechanism explicitly here
	(the first literal of any FFI spec'ed method is an ExternalFunction
	and not an array as used in the pluggable primitive mechanism). */
/*	Manually copied from primitiveCalloutAccessorDepth in the
	ThreadedFFIPlugins... 
 */
/*	since call may invoke a callback */
/*	since call signature is in first literal... */

	/* InterpreterPrimitives>>#primitiveCalloutToFFI */

static void
primitiveCalloutToFFI(void)
{
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
		if (!primFailCode) {
			primFailCode = 1;
		}
	}
}