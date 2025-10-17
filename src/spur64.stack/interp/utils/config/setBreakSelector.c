/* Extracted from interp.c:62972 (function setBreakSelector). */

/*	useful for VM debugging */

/* StackInterpreter>>#setBreakSelector: */

void setBreakSelector(char *aString) {
  if ((breakSelector = aString)) {
    breakSelectorLength = strlen(aString);
  } else {
    breakSelectorLength = MinSmallInteger;
  }
}