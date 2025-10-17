/* Extracted from interp.c:62957 (function setBreakMNUSelector). */

/*	useful for VM debugging */

/* StackInterpreter>>#setBreakMNUSelector: */

void setBreakMNUSelector(char *aString) {
  if ((breakSelector = aString)) {
    breakSelectorLength = -(strlen(aString));
  } else {
    breakSelectorLength = MinSmallInteger;
  }
}