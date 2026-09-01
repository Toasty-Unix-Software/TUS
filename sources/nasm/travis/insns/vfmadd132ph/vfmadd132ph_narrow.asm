	vfmadd132ph xmm4, xmm5, xmm5
	vfmadd132ph xmm4, xmm5
	vfmadd132ph xmm7, xmm3, oword [0x1f3]
	vfmadd132ph ymm2, ymm7, yword [0xf14]
	vfmadd132ph ymm2, yword [0xf14]
	vfmadd132ph ymm5, ymm3, ymm3
	vfmadd132ph zmm5, zmm6, zmm0
	vfmadd132ph zmm5, zmm0
	vfmadd132ph zmm4, zmm0, zmm5

%ifdef ERROR
	vfmadd132ph xmm12, xmm11, xmm9
	vfmadd132ph ymm10, ymm10, ymm15
	vfmadd132ph zmm8, zmm11, zmm13
	vfmadd132ph xmm25, xmm29, xmm18
	vfmadd132ph ymm19, ymm20, ymm30
	vfmadd132ph zmm16, zmm18, zmm27
%endif
