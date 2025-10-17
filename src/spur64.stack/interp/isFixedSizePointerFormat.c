/* Extracted from interp.c:37278 (function isFixedSizePointerFormat). */

	/* SpurMemoryManager>>#isFixedSizePointerFormat: */

sqInt
isFixedSizePointerFormat(sqInt format)
{
	return (format <= (nonIndexablePointerFormat()))
	 || (format == (ephemeronFormat()));
}