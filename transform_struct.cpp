#include <mex.h>
#include <matrix.h>
#include <string.h>
#include <mexutil.hpp>
#include <callback.hpp>
#include <vector>


// TODO: contains memory leaks (when a mexErrMsg function is called, there might
// still exist the cb_list
//


/*
 * sample file that shows how to transform an array of structs
 * containing (id, handle) pairs to a callback list and return the
 * pointer to the data. the callback list can thus be passed to the
 * second file in this example (exec_callbacks.cpp).
 *
 * The passed matlab functions must not have any argument as this example here
 * doesn't take care of those arguments.
 */
void
mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	if (nrhs != 1)
		mexErrMsgTxt("Function requires exactly one argument.\n");
	if (nlhs != 1)
		mexErrMsgTxt("Function returns exactly one value.\n");

	if (!mxIsStruct(prhs[0]))
		mexErrMsgTxt("Function requires a structure array as argument\n");

	int nfields = mxGetNumberOfFields(prhs[0]);
	mwSize nstructs = mxGetNumberOfElements(prhs[0]);

	std::vector<callback*> *cb_list = new std::vector<callback*>();
	// parse each struct
	for (mwIndex i = 0; i < nstructs; i++) {
		bool id_found = false;
		bool fn_found = false;

		callback *cb = new callback();
		for (int j = 0; j < nfields; j++) {
			mxArray *field = mxGetFieldByNumber(prhs[0], i, j);
			if (!field) {
				mexWarnMsgTxt("Invalid structure found\n");
				continue;
			}

			const char *fname = mxGetFieldNameByNumber(prhs[0], j);
			if (!strncmp(fname, "id", sizeof("id"))) {
				if (!mxIsNumeric(field) || mxGetNumberOfElements(field) != 1) {
					mexWarnMsgTxt("Invalid id (must be numeric scalar).\n");
					continue;
				}
				id_found = true;
				cb->id = (int)mxGetScalar(field);
			}
			else if (!strncmp(fname, "handle", sizeof("handle"))) {
				if (!mxIsClass(field, "function_handle")) {
					mexWarnMsgTxt("Invalid handle (must be function_handle).\n");
					continue;
				}
				fn_found = true;

				cb->fn = field;
			}
		}

		if (!id_found || !fn_found)
			delete cb;
		else
			cb_list->push_back(cb);
	}
	// return the list
	plhs[0] = create_mex_obj(cb_list);
}
