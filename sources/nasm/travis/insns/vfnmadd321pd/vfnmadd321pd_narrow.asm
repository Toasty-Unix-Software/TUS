	vfnmadd321pd xmm5, xmm4, oword [0x28c]
	vfnmadd321pd xmm7, xmm3, xmm4
	vfnmadd321pd ymm4, ymm1, ymm2
	vfnmadd321pd ymm1, ymm5, ymm3

%ifdef ERROR
	vfnmadd321pd xmm8, xmm9, xmm15
	vfnmadd321pd ymm12, ymm14, ymm11
%endif
