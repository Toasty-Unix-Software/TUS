	vfmadd312ps xmm6, xmm0, xmm3
	vfmadd312ps xmm0, xmm6, xmm3
	vfmadd312ps ymm1, ymm1, yword [0x54b]
	vfmadd312ps ymm4, ymm4, yword [0xdf4]

%ifdef ERROR
	vfmadd312ps xmm8, xmm12, xmm8
	vfmadd312ps ymm11, ymm15, ymm9
%endif
