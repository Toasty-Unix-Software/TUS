	vfnmadd231ps xmm3, xmm1, xmm7
	vfnmadd231ps xmm0, xmm7, xmm6
	vfnmadd231ps ymm5, ymm6, ymm4
	vfnmadd231ps ymm6, ymm3, yword [0xac9]
	vfnmadd231ps xmm7, xmm7, xmm3
	vfnmadd231ps xmm5, xmm1, xmm3
	vfnmadd231ps ymm5, ymm7, ymm5
	vfnmadd231ps ymm5, ymm3, yword [0xdfe]

%ifdef ERROR
	vfnmadd231ps xmm12, xmm11, xmm14
	vfnmadd231ps ymm10, ymm15, ymm14
	vfnmadd231ps xmm13, xmm11, xmm8
	vfnmadd231ps ymm11, ymm13, ymm11
	vfnmadd231ps xmm26, xmm24, xmm24
	vfnmadd231ps ymm16, ymm22, ymm30
	vfnmadd231ps xmm21, xmm29, xmm28
	vfnmadd231ps ymm27, ymm18, ymm24
%endif
