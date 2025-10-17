/* Extracted from interp.c:37452 (function isMaybeFiredEphemeron). */

	/* SpurMemoryManager>>#isMaybeFiredEphemeron: */

static NoDbgRegParms sqInt
isMaybeFiredEphemeron(sqInt objOop)
{
    sqInt format;

	format = (byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask());

	/* begin isMaybeFiredEphemeronFormat: */
	return (format <= 5 /* lastPointerFormat */)
	 && ((format & 1));
}