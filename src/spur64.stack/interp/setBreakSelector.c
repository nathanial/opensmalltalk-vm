/* Extracted from interp.c:62972 (function setBreakSelector). */

void
setBreakSelector(char *aString)
{
	if ((breakSelector = aString)) {
		breakSelectorLength = strlen(aString);
	}
	else {
		breakSelectorLength = MinSmallInteger;
	}
}