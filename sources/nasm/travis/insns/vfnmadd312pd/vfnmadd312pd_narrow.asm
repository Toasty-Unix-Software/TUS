	vfnmadd312pd xmm3, xmm2, oword [0x3b3]
	vfnmadd312pd xmm3, xmm0, xmm0
	vfnmadd312pd ymm3, ymm2, ymm5
	vfnmadd312pd ymm0, ymm1, ymm5

%ifdef ERROR
	vfnmadd312pd xmm15, xmm9, xmm15
	vfnmadd312pd ymm9, ymm14, ymm13
%endif
