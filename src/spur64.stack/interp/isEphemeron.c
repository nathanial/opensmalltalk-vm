/* Extracted from interp.c:37265 (function isEphemeron). */

	/* SpurMemoryManager>>#isEphemeron: */

static NoDbgRegParms int
isEphemeron(sqInt objOop)
{
    sqInt format;

	assert(isNonImmediate(objOop));
	format = (byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask());

	/* begin isEphemeronFormat: */
	return format == (ephemeronFormat());
}