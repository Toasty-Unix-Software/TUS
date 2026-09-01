	vfnmadd132bf16 xmm0, xmm3, xmm5
	vfnmadd132bf16 xmm3, xmm7, oword [0xf1a]
	vfnmadd132bf16 ymm3, ymm1, yword [0x53f]
	vfnmadd132bf16 ymm5, ymm0, yword [0x356]
	vfnmadd132bf16 zmm7, zmm1, zword [0xf2d]
	vfnmadd132bf16 zmm3, zmm2, zword [0x559]

%ifdef ERROR
	vfnmadd132bf16 xmm14, xmm13, xmm14
	vfnmadd132bf16 ymm15, ymm8, ymm12
	vfnmadd132bf16 zmm15, zmm9, zmm15
	vfnmadd132bf16 xmm25, xmm16, xmm25
	vfnmadd132bf16 ymm29, ymm30, ymm20
	vfnmadd132bf16 zmm23, zmm23, zmm16
%endif
