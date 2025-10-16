/* Extracted from interp.c:50820 (function checkInterpreterIntegrity). */

static sqInt
checkInterpreterIntegrity(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt flags;

	flags = 0;
	if (!(checkOopIntegritynamed(GIV(specialObjectsOop), "specialObjectsOop"))) {
		flags = 1;
	}

	/* No longer check messageSelector; it is ephemeral, not living beyond message lookup.
	   (objectMemory isNonImmediate: messageSelector) ifTrue:
	   [(objectMemory checkOopIntegrity: messageSelector named: 'messageSelector') ifFalse:
	   [flags := flags + N]]. */
	if (!(checkOopIntegritynamed(GIV(newMethod), "newMethod"))) {
		flags += 2;
	}

	/* No longer check lkupClass; it is ephemeral, not living beyond message lookup.
	   (objectMemory checkOopIntegrity: lkupClass named: 'lkupClass') ifFalse:
	   [flags := flags + N]. */
	if (GIV(profileProcess)) {
		if (!(checkOopIntegritynamed(GIV(profileProcess), "profileProcess"))) {
			flags += 4;
		}
	}
	if (GIV(profileMethod)) {
		if (!(checkOopIntegritynamed(GIV(profileMethod), "profileMethod"))) {
			flags += 8;
		}
	}
	if (GIV(profileSemaphore)) {
		if (!(checkOopIntegritynamed(GIV(profileSemaphore), "profileSemaphore"))) {
			flags += 16;
		}
	}
	if (GIV(tempOop)) {
		if (!(checkOopIntegritynamed(GIV(tempOop), "tempOop"))) {
			flags += 32;
		}
	}
	if (GIV(tempOop2)) {
		if (!(checkOopIntegritynamed(GIV(tempOop2), "tempOop2"))) {
			flags += 64;
		}
	}
	if (!(checkLogIntegrity())) {
		flags += 128;
	}
	return flags;
}