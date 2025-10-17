/* Extracted from interp.c:45705 (function statNumGCs). */

/*	Part of InterpreterProxy's 1.14 API */

/* SpurMemoryManager>>#statNumGCs */

sqInt statNumGCs(void) { return (statScavenges + statIncrGCs) + statFullGCs; }