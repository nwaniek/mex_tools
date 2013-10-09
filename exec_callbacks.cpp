#include <mex.h>
#include <matrix.h>
#include <string.h>
#include <mexutil.hpp>
#include <callback.hpp>
#include <vector>


/*
 * sample function that shows how to retrieve a callback list, which was previously
 * created using the function provided by transform_struct.cpp.
 * Each of the functions will be executed using matlab (with the help of the
 * feval function) in sequence.
 *
 * The passed matlab functions must not have any argument as this example here
 * doesn't take care of those arguments.
 */
void
mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	typedef std::vector<callback*> cb_list_t;

	if (nrhs != 1)
		mexErrMsgTxt("Function requires exactly one argument.\n");


	cb_list_t *cb_list = retrieve_mex_obj<cb_list_t>(prhs[0]);
	if (!cb_list)
		mexErrMsgTxt("Invalid callback list\n");

	for (cb_list_t::iterator it = cb_list->begin(); it != cb_list->end(); ++it) {
		// the example will transform a string, and yield a result
		mxArray *result = (*it)->fn(1, mxCreateString("world"));
		mexPrintf("%s\n", mxArrayToString(result));
	}
	// cb->fn(1, mxCreateString("world"));
}
