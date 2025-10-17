/* Extracted from interp.c:28494 (function openScavengeLog). */

/* SpurGenerationScavenger>>#openScavengeLog */

void openScavengeLog(void) { scavengeLog = fopen("scavenge.log", "a+"); }