	vfmadd231ph xmm6, xmm7, oword [0xc7c]
	vfmadd231ph xmm6, oword [0xc7c]
	vfmadd231ph xmm2, xmm5, xmm0
	vfmadd231ph ymm6, ymm1, ymm5
	vfmadd231ph ymm6, ymm5
	vfmadd231ph ymm3, ymm7, ymm6
	vfmadd231ph zmm6, zmm3, zmm6
	vfmadd231ph zmm6, zmm6
	vfmadd231ph zmm7, zmm2, zmm3

%ifdef ERROR
	vfmadd231ph xmm14, xmm10, xmm10
	vfmadd231ph ymm14, ymm10, ymm8
	vfmadd231ph zmm14, zmm11, zmm11
	vfmadd231ph xmm28, xmm17, xmm16
	vfmadd231ph ymm24, ymm20, ymm20
	vfmadd231ph zmm30, zmm16, zmm21
%endif
