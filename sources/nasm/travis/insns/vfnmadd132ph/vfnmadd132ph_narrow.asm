	vfnmadd132ph xmm1, xmm7, oword [0xf73]
	vfnmadd132ph xmm1, oword [0xf73]
	vfnmadd132ph xmm2, xmm7, xmm0
	vfnmadd132ph ymm6, ymm1, yword [0xd9a]
	vfnmadd132ph ymm6, yword [0xd9a]
	vfnmadd132ph ymm4, ymm2, ymm7
	vfnmadd132ph zmm4, zmm5, zmm6
	vfnmadd132ph zmm4, zmm6
	vfnmadd132ph zmm7, zmm0, zword [0x6c6]

%ifdef ERROR
	vfnmadd132ph xmm10, xmm12, xmm9
	vfnmadd132ph ymm8, ymm12, ymm10
	vfnmadd132ph zmm12, zmm14, zmm12
	vfnmadd132ph xmm20, xmm22, xmm20
	vfnmadd132ph ymm19, ymm18, ymm19
	vfnmadd132ph zmm19, zmm23, zmm24
%endif
