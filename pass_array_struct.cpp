#include <mex.h>
#include <matrix.h>
#include <string.h>


void
call_10times(mxArray *fn)
{
	for (int i = 0; i < 10; i++)
		mexCallMATLAB(0, NULL, 1, &fn, "feval");
}


void
mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	if (nrhs < 1)
		mexErrMsgTxt("Function expects exactly one argument: array of structs containing (id, handle) pairs\n");

	if (!mxIsStruct(prhs[0]))
		mexErrMsgTxt("Argument must be a structure array containing (id, handle) pairs\n");

	// get number of fields in the struct
	int nfields = mxGetNumberOfFields(prhs[0]);
	// get number of structs passed
	mwSize nstructs = mxGetNumberOfElements(prhs[0]);

	// print some information
	mexPrintf("structs: %d, fields per struct: %d\n", nstructs, nfields);
	// for (mwIndex i = 0; i < nstructs; i++) {
	mwIndex i = 0;
		for (int j = 0; j < nfields; j++) {
			mxArray *field = mxGetFieldByNumber(prhs[0], i, j);
			if (!field) {
				mexErrMsgTxt("Got an error here\n");
			}
			const char *fname = mxGetFieldNameByNumber(prhs[0], j);

			// extract id
			if (!strncmp(fname, "id", sizeof("id"))) {
				if (!mxIsNumeric(field) || mxGetNumberOfElements(field) != 1)
					mexErrMsgTxt("invalid id (must be scalar and numeric)\n");

				int d = (int)mxGetScalar(field);
				mexPrintf("id = %d\n", d);
			}
			// extract handles
			else if (!strncmp(fname, "handle", sizeof("handle"))) {
				if (!mxIsClass(field, "function_handle"))
					mexErrMsgTxt("invalid function handle\n");

				call_10times(field);
				const char *bla[1] = {"handle"};
				plhs[0] = mxCreateStructMatrix(1, 1, 1, bla);

				char *s = mxArrayToString(field);

				// don't forget the duplicate array!
				mxSetFieldByNumber(plhs[0], 0, 0, mxDuplicateArray(field));
			}
		}
	// }
}
