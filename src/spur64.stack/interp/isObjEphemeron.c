/* Extracted from interp.c:37472 (function isObjEphemeron). */

static NoDbgRegParms int
isObjEphemeron(sqInt objOop)
{
    sqInt format;

	format = (byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask());

	/* begin isEphemeronFormat: */
	return format == (ephemeronFormat());
}