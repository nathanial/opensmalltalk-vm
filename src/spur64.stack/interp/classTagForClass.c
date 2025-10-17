/* Extracted from interp.c:33398 (function classTagForClass). */

/*	Answer the classObj's identityHash to use as a tag in the first-level
	method lookup cache.
 */

	/* SpurMemoryManager>>#classTagForClass: */

sqInt
classTagForClass(sqInt classObj)
{
    sqInt err;
    sqInt hash;

	/* begin ensureBehaviorHash: */
	assert(addressCouldBeClassObj(classObj));

	/* eem 12/28/2021 the above asserft is too weak (and only an assert) */
	return ((hash = (long32At((void *)(classObj + 4))) & (identityHashHalfWordMask()))
			? hash
			: (objCouldBeClassObj(classObj)
					? ((err = enterIntoClassTable(classObj))
							? -err
							: (long32At((void *)(classObj + 4))) & (identityHashHalfWordMask()))
					: -PrimErrBadReceiver));
}