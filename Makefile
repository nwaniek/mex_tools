MEX 	 = ${MATLAB_ROOT}/bin/mex
PLTFORM  = -DPLATFORM_X86_64
MEXEXT   = $(shell $(MEX)ext)
MEXFLAGS = -cxx -I${MATLAB_ROOT}/extern/include -I./ $(PLTFORM)


all: pass_array_struct call_fhandle transform_struct exec_callbacks \
	expandword createstruct

pass_array_struct:
	$(MEX) $(MEXFLAGS) $@.cpp

call_fhandle:
	$(MEX) $(MEXFLAGS) $@.cpp

transform_struct:
	$(MEX) $(MEXFLAGS) $@.cpp

exec_callbacks:
	$(MEX) $(MEXFLAGS) $@.cpp

expandword:
	$(MEX) $(MEXFLAGS) $@.cpp

createstruct:
	$(MEX) $(MEXFLAGS) $@.cpp

clean:
	rm -f *.$(MEXEXT)
