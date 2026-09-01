	vfmadd231ps xmm5, xmm7, xmm0
	vfmadd231ps xmm2, xmm4, xmm0
	vfmadd231ps ymm0, ymm6, yword [0x5aa]
	vfmadd231ps ymm2, ymm5, ymm5
	vfmadd231ps xmm2, xmm5, xmm4
	vfmadd231ps xmm7, xmm2, xmm2
	vfmadd231ps ymm0, ymm0, ymm3
	vfmadd231ps ymm6, ymm0, yword [0x14d]

%ifdef ERROR
	vfmadd231ps xmm10, xmm11, xmm13
	vfmadd231ps ymm13, ymm14, ymm15
	vfmadd231ps xmm9, xmm9, xmm14
	vfmadd231ps ymm8, ymm9, ymm12
	vfmadd231ps xmm26, xmm31, xmm31
	vfmadd231ps ymm29, ymm20, ymm16
	vfmadd231ps xmm20, xmm16, xmm31
	vfmadd231ps ymm31, ymm20, ymm17
%endif
