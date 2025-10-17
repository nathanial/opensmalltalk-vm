/* Extracted from interp.c:34625 (function ensureBehaviorHash). */

	/* SpurMemoryManager>>#ensureBehaviorHash: */

static NoDbgRegParms sqInt
ensureBehaviorHash(sqInt aBehavior)
{
    sqInt err;
    sqInt hash;

	assert(addressCouldBeClassObj(aBehavior));

	/* eem 12/28/2021 the above asserft is too weak (and only an assert) */
	return ((hash = (long32At((void *)(aBehavior + 4))) & (identityHashHalfWordMask()))
			? hash
			: (objCouldBeClassObj(aBehavior)
					? ((err = enterIntoClassTable(aBehavior))
							? -err
							: (long32At((void *)(aBehavior + 4))) & (identityHashHalfWordMask()))
					: -PrimErrBadReceiver));
}