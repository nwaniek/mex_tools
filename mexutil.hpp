/*
 * This file includes template functions to store regular object pointers in a
 * matlab type so that it can be passed to different MEX files.
 *
 * MAKE SURE TO COMPILE YOUR MEX FILES AS C++ CODE!
 *
 */
#ifndef __MEXUTIL_HPP__C1840E12_1B83_49C3_B8B2_D27A6B61D8E4
#define __MEXUTIL_HPP__C1840E12_1B83_49C3_B8B2_D27A6B61D8E4

#include <mex.h>


// TODO: adapt Tim Bailey and Mike Stevens pattern for easy grabage collection
// and a little more runtime checking of the object type


// determine the pointer size to use. Assumption is that one wants to use a
// 64bit pointer on x64 platforms. (as experiments have shown, it would work
// with 32bit pointers as well. If any problems occure, try to change the
// POINTER_CLASS
#ifdef PLATFORM_X86_64
#	define POINTER_CLASS mxUINT64_CLASS
#else
#	define POINTER_CLASS mxUINT32_CLASS
#endif


/**
 * The function create_mex_obj will create a new NumericMatrix element that will
 * store the pointer to the object that is passed into it. It may therefore be
 * used in Matlab/MEX, e.g. as a result to a function and as input into another
 * function.
 *
 * Don't forget to delete Object's you created in a MEX file!
 *
 * Example MEX source to create a new mex obj
 *
 *	#include <mex.h>
 *	#include <mexutil.hpp>
 *
 *	void
 *	mexFunction (int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])
 *	{
 *		// do input checking
 *
 *		// create the object that shall be passed out to matlab
 *		ObjType *obj = new ObjType(param1, param2, ...);
 *
 *		// create the mex object
 *		plhs[0] = create_mex_obj(obj);
 *
 *		// do anything additional you wish to do with the object
 *	}
 *
 */
template <typename T>
mxArray *
create_mex_obj (T* t)
{
	mxArray *ptr = mxCreateNumericMatrix(1, 1, POINTER_CLASS, mxREAL);
	*reinterpret_cast<T**>(mxGetPr(ptr)) = t;
	return ptr;
}


/**
 * This function will try to retrieve an object from a matlab NumericMatrix
 * element. There are some cases that this might not work, e.g. when wrong
 * parameters are passed. If so, the function will fail.
 *
 * Example MEX source to use a mex obj that was passed into a function:
 *
 *	#include <mex.h>
 *	#include <mexuti.hpp>
 *
 *	void
 *	mexFunction (int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])
 *	{
 *		// input checking, 1 argument (the object) assumed
 *		if (nrhs != 1)
 *			mexErrMsgTxt("Function requires exactly one argument.");
 *
 *		// now convert the prhs[0] element to the object
 *		ObjType *obj = retrieve_mex_obj<ObjType>(prhs[0]);
 *
 *		// do anything with the object that is required
 *		obj->method1();
 *		obj->method2();
 *	}
 *
 */
template <typename T>
T*
retrieve_mex_obj (const mxArray *ptr)
{
	bool valid_ptr = (mxGetClassID(ptr) == POINTER_CLASS)
		&& !mxIsComplex(ptr) && mxGetM(ptr) == 1 && mxGetN(ptr) == 1;

	if (!valid_ptr)
		mexErrMsgTxt("Parameter is not a valid Object Pointer.");

	T* obj = *reinterpret_cast<T**>(mxGetPr(ptr));
	if (!obj)
		mexErrMsgTxt("Parameter is NULL.");

	return obj;
}


/**
 * This function will provide an easy way to delete objects that are stored in a
 * Matlab NumericMatrix element. Usually this is called in a finalization
 * function when an algorithm is done. If you won't make sure that mex objects
 * will be deleted, you will have a memory leak!
 *
 * Example MEX source to kill an object
 *
 *	#include <mex.h>
 *	#include <mexutil.hpp>
 *
 *	void
 *	mexFunction (int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])
 *	{
 *		// assumed one argument, the mex object
 *		if (nrhs != 1)
 *			mexErrMsgTxt("Function requires exactly one argument.");
 *
 *		delete_mex_obj<ObjType>(prhs[0]);
 *	}
 *
 */
template <typename T>
void
delete_mex_obj (const mxArray *ptr)
{
	T *obj = retrieve_mex_obj<T>(ptr);
	delete obj;
}


#endif /* __MEXUTIL_HPP__C1840E12_1B83_49C3_B8B2_D27A6B61D8E4 */

