	vfnmadd123ps xmm5, xmm0, xmm4
	vfnmadd123ps xmm3, xmm2, xmm1
	vfnmadd123ps ymm0, ymm5, yword [0x57b]
	vfnmadd123ps ymm4, ymm5, yword [0x3c6]

%ifdef ERROR
	vfnmadd123ps xmm11, xmm8, xmm12
	vfnmadd123ps ymm14, ymm13, ymm15
%endif
