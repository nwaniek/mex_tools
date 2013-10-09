#include <mex.h>
#include <string.h>
#include <wordexp.h>
#include <string>

/*
 * This function expands words using the wordexp function from the Standard
 * C library. If no argument is passed, it simply returns the expansion
 * of $HOME. It does, however, not return the expanded string. It just
 * prints it as an example that this works.
 *
 * Matlab example code:
 *	expandword()		% will print your home directory
 *	expandword('$PATH')	% will print your PATH environment variable
 */
void
mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	// this needs to be a string that is set in your environment
	// variables. i.e. $HOME
	char *s = "$HOME";
	if (nrhs > 0 && mxIsChar(prhs[0])) {
		mexPrintf("something passed\n");
		s = (char*)mxCalloc(mxGetN(prhs[0])+1, sizeof(char));
		mxGetString(prhs[0], s, mxGetN(prhs[0]) + 1);
	}

	wordexp_t p;
	char **w;

	wordexp(s, &p, 0);
	w = p.we_wordv;
	for (int i = 0; i < p.we_wordc; i++)
		mexPrintf("%s\n", w[i]);
	wordfree(&p);
}
