	vfmadd132ps xmm1, xmm4, xmm3
	vfmadd132ps xmm7, xmm2, oword [0xa90]
	vfmadd132ps ymm2, ymm5, ymm4
	vfmadd132ps ymm0, ymm7, ymm5
	vfmadd132ps xmm3, xmm5, xmm7
	vfmadd132ps xmm4, xmm5, xmm2
	vfmadd132ps ymm6, ymm3, ymm5
	vfmadd132ps ymm5, ymm0, ymm6

%ifdef ERROR
	vfmadd132ps xmm8, xmm10, xmm12
	vfmadd132ps ymm13, ymm12, ymm13
	vfmadd132ps xmm11, xmm10, xmm10
	vfmadd132ps ymm15, ymm11, ymm10
	vfmadd132ps xmm25, xmm17, xmm27
	vfmadd132ps ymm25, ymm21, ymm19
	vfmadd132ps xmm23, xmm23, xmm28
	vfmadd132ps ymm16, ymm28, ymm25
%endif
