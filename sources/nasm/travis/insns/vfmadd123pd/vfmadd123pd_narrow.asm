	vfmadd123pd xmm7, xmm1, xmm1
	vfmadd123pd xmm2, xmm7, xmm4
	vfmadd123pd ymm6, ymm7, ymm2
	vfmadd123pd ymm2, ymm1, ymm0

%ifdef ERROR
	vfmadd123pd xmm15, xmm8, xmm14
	vfmadd123pd ymm14, ymm15, ymm15
%endif
