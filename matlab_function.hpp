#ifndef __MATLAB_FUNCTION_HPP__102E0496_A635_40D2_8D94_D53D98B8AB49
#define __MATLAB_FUNCTION_HPP__102E0496_A635_40D2_8D94_D53D98B8AB49

#include <mex.h>
#include <stdarg.h>

/*
 * class to wrap a matlab function handle easily for use from different MEX
 * calls
 */
struct matlab_function {

// constructors, destructors

	matlab_function () : fn(NULL) {}

	matlab_function (const mxArray *fn) : fn(NULL) {
		this->fn = mxDuplicateArray(fn);
		mexMakeArrayPersistent(this->fn);
	}

	matlab_function (const matlab_function &fn) : fn(NULL) {
		this->fn = mxDuplicateArray(fn.fn);
		mexMakeArrayPersistent(this->fn);
	}

	~matlab_function () {
		if (fn) {
			mxDestroyArray(this->fn);
			this->fn = NULL;
		}
	}


// assignment operators

	matlab_function& operator= (const matlab_function &rhs) {
		if (fn)
			mxDestroyArray(this->fn);
		this->fn = mxDuplicateArray(rhs.fn);
		mexMakeArrayPersistent(this->fn);
		return *this;
	}

	matlab_function& operator= (const matlab_function *rhs) {
		if (fn)
			mxDestroyArray(this->fn);
		this->fn = mxDuplicateArray(rhs->fn);
		mexMakeArrayPersistent(this->fn);
		return *this;
	}

	matlab_function& operator= (const mxArray *rhs) {
		if (fn) {
			mxDestroyArray(this->fn);
			this->fn = NULL;
		}
		if (mxIsEmpty(rhs) || !mxIsClass(rhs, "function_handle"))
			return *this;

		this->fn = mxDuplicateArray(rhs);
		mexMakeArrayPersistent(this->fn);
		return *this;
	}


// function evaluation

	/**
	 * call the matlab function with a variadic number of arguments. the
	 * matlab function must return exactly one result. If you want to change
	 * the internal behaviour of operator(), simply derive from this class
	 * and implement operator() on your own
	 */
	virtual mxArray* operator() (unsigned n, ...)
	{
		if (!this->fn)
			return NULL;

		mxArray *lhs[1];
		mxArray *rhs[n+1];
		rhs[0] = this->fn;

		va_list ap;
		va_start(ap, n);
		for (int i = 0; i < n; i++)
			rhs[i+1] = va_arg(ap, mxArray*);
		va_end(ap);

		mexCallMATLAB(1, lhs, n+1, rhs, "feval");
		return lhs[0];
	}

private:
	mxArray *fn;
};

#endif /* __MATLAB_FUNCTION_HPP__102E0496_A635_40D2_8D94_D53D98B8AB49 */

