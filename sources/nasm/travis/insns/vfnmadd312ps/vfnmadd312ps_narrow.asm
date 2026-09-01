	vfnmadd312ps xmm0, xmm6, oword [0x189]
	vfnmadd312ps xmm2, xmm4, xmm7
	vfnmadd312ps ymm4, ymm7, ymm3
	vfnmadd312ps ymm4, ymm3, ymm7

%ifdef ERROR
	vfnmadd312ps xmm13, xmm10, xmm8
	vfnmadd312ps ymm9, ymm12, ymm9
%endif
