/* Extracted from interp.c:55152 (function isQuickPrimitiveIndex). */

int
isQuickPrimitiveIndex(sqInt anInteger)
{
	return ((anInteger >= 0x100) && (anInteger <= 519));
}