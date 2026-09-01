	vfmadd312pd xmm7, xmm1, xmm3
	vfmadd312pd xmm2, xmm7, xmm2
	vfmadd312pd ymm2, ymm3, ymm2
	vfmadd312pd ymm0, ymm7, ymm3

%ifdef ERROR
	vfmadd312pd xmm14, xmm15, xmm9
	vfmadd312pd ymm15, ymm15, ymm15
%endif
