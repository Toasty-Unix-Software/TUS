	vfnmadd231ph xmm6, xmm3, xmm2
	vfnmadd231ph xmm6, xmm2
	vfnmadd231ph xmm4, xmm0, xmm1
	vfnmadd231ph ymm0, ymm7, ymm3
	vfnmadd231ph ymm0, ymm3
	vfnmadd231ph ymm6, ymm5, ymm6
	vfnmadd231ph zmm2, zmm3, zword [0xace]
	vfnmadd231ph zmm2, zword [0xace]
	vfnmadd231ph zmm0, zmm5, zword [0xf2d]

%ifdef ERROR
	vfnmadd231ph xmm9, xmm14, xmm8
	vfnmadd231ph ymm14, ymm10, ymm12
	vfnmadd231ph zmm13, zmm9, zmm8
	vfnmadd231ph xmm16, xmm26, xmm19
	vfnmadd231ph ymm28, ymm22, ymm20
	vfnmadd231ph zmm23, zmm25, zmm17
%endif
