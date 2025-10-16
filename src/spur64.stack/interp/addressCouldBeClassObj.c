/* Extracted from interp.c:29729 (function addressCouldBeClassObj). */

static NoDbgRegParms sqInt
addressCouldBeClassObj(sqInt maybeClassObj)
{
	return (addressCouldBeObj(maybeClassObj))
	 && (objCouldBeClassObj(maybeClassObj));
}