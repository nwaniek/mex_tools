#include <mex.h>
#include <matrix.h>
#include <string.h>


/*
 * This function is just a very small showcase how to invoke function
 * handles from within a MEX extensions. This function will take exactly
 * one argument, a function handle that does not return any value. it
 * subsequently calls the function handle.
 *
 * Matlab sample code:
 *	x = @() disp('hello world');
 *	call_fhandle(x);
 *
 */
void
mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	if (nrhs <= 0)
		mexErrMsgTxt("Function requires exactly one argument\n");

	if (!mxIsClass(prhs[0], "function_handle"))
		mexErrMsgTxt("First argument needs to be a function handle\n");

	mxArray *fn[1];
	fn[0] = const_cast<mxArray*>(prhs[0]);
	mexCallMATLAB(0, NULL, 1, fn, "feval");
}
