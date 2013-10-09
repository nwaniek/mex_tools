#include <mex.h>
#include <matrix.h>

#define LENGTH(X) (sizeof (X) / sizeof (X)[0])

/*
 * This function simply creates an array of structs, each struct having
 * a few fields
 */
void
mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	if (nlhs != 1)
		mexErrMsgTxt("Invalid number of left-hand side arguments.");

	const int nelems = 10;
	const char *fnames[] = {"id", "handle"};
	plhs[0] = mxCreateStructMatrix(nelems, 1, LENGTH(fnames), fnames);
	for (int i = 0; i < nelems; i++)
		mxSetFieldByNumber(plhs[0], i, 0, mxCreateDoubleScalar(i));
}


