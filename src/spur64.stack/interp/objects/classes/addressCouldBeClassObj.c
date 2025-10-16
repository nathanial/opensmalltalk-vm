/* Extracted from interp.c:29729 (function addressCouldBeClassObj). */

/*	Answer if maybeClassObj looks like a class object */

/* SpurMemoryManager>>#addressCouldBeClassObj: */

static sqInt addressCouldBeClassObj(sqInt maybeClassObj) {
  return (addressCouldBeObj(maybeClassObj)) &&
         (objCouldBeClassObj(maybeClassObj));
}