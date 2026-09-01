	vfmadd321pd xmm5, xmm0, xmm2
	vfmadd321pd xmm6, xmm0, xmm7
	vfmadd321pd ymm2, ymm2, yword [0xb9b]
	vfmadd321pd ymm1, ymm3, ymm7

%ifdef ERROR
	vfmadd321pd xmm15, xmm13, xmm8
	vfmadd321pd ymm10, ymm12, ymm9
%endif
