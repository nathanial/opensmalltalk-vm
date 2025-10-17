/* Extracted from interp.c:50820 (function checkInterpreterIntegrity). */

/*	Perform an integrity/leak check using the heapMap. Assume
        clearLeakMapAndMapAccessibleObjects has set a bit at each
        object's header. Check that all oops in the interpreter's state
        points to a header. Answer 0 if all checks pass. */

/* StackInterpreter>>#checkInterpreterIntegrity */

static sqInt checkInterpreterIntegrity(void) {
  sqInt flags;

  flags = 0;
  if (!(checkOopIntegritynamed(specialObjectsOop, "specialObjectsOop"))) {
    flags = 1;
  }

  /* No longer check messageSelector; it is ephemeral, not living beyond message
     lookup. (objectMemory isNonImmediate: messageSelector) ifTrue:
     [(objectMemory checkOopIntegrity: messageSelector named: 'messageSelector')
     ifFalse: [flags := flags + N]]. */
  if (!(checkOopIntegritynamed(newMethod, "newMethod"))) {
    flags += 2;
  }

  /* No longer check lkupClass; it is ephemeral, not living beyond message
     lookup. (objectMemory checkOopIntegrity: lkupClass named: 'lkupClass')
     ifFalse: [flags := flags + N]. */
  if (profileProcess) {
    if (!(checkOopIntegritynamed(profileProcess, "profileProcess"))) {
      flags += 4;
    }
  }
  if (profileMethod) {
    if (!(checkOopIntegritynamed(profileMethod, "profileMethod"))) {
      flags += 8;
    }
  }
  if (profileSemaphore) {
    if (!(checkOopIntegritynamed(profileSemaphore, "profileSemaphore"))) {
      flags += 16;
    }
  }
  if (tempOop) {
    if (!(checkOopIntegritynamed(tempOop, "tempOop"))) {
      flags += 32;
    }
  }
  if (tempOop2) {
    if (!(checkOopIntegritynamed(tempOop2, "tempOop2"))) {
      flags += 64;
    }
  }
  if (!(checkLogIntegrity())) {
    flags += 128;
  }
  return flags;
}