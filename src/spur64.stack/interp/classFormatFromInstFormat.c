/* Extracted from interp.c:33230 (function classFormatFromInstFormat). */

static NoDbgRegParms sqInt
classFormatFromInstFormat(sqInt instFormat)
{
	if (instFormat >= (firstByteFormat())) {
		return instFormat & -8;
	}

	/* this is likely the common case */
	if (instFormat <= (sixtyFourBitIndexableFormat())) {
		return instFormat;
	}
	if (instFormat < (firstShortFormat())) {
		return instFormat & -2;
	}
	return instFormat & -4;
}