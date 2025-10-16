/* Extracted from interp.c:62957 (function setBreakMNUSelector). */

void
setBreakMNUSelector(char *aString)
{
	if ((breakSelector = aString)) {
		breakSelectorLength = -(strlen(aString));
	}
	else {
		breakSelectorLength = MinSmallInteger;
	}
}