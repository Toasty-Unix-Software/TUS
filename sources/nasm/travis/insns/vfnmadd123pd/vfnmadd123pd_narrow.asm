	vfnmadd123pd xmm0, xmm2, oword [0x5a1]
	vfnmadd123pd xmm5, xmm7, xmm0
	vfnmadd123pd ymm4, ymm2, ymm0
	vfnmadd123pd ymm6, ymm1, ymm3

%ifdef ERROR
	vfnmadd123pd xmm10, xmm8, xmm15
	vfnmadd123pd ymm12, ymm13, ymm10
%endif
