/* Extracted from interp.c:37175 (function isCharacterValue). */

int
isCharacterValue(sqInt anInteger)
{
	return ((anInteger >= 0) && (anInteger <= (0x3FFFFFFF)));
}