/* Extracted from interp.c:49900 (function addIdleUsecs). */

/*	The various poll/select calls in the VM should attempt to tally the
        ammount of time spent at idle here, so as to render the uptime value
        meaningful.
 */

/* StackInterpreter>>#addIdleUsecs: */

void addIdleUsecs(sqInt idleUsecs) { statIdleUsecs += idleUsecs; }