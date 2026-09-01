default rel
	vfnmadd132bf16 xmm0, xmm3, xmm5
	vfnmadd132bf16 xmm3, xmm7, oword [0xf1a]
	vfnmadd132bf16 ymm3, ymm1, yword [0x53f]
	vfnmadd132bf16 ymm5, ymm0, yword [0x356]
	vfnmadd132bf16 zmm7, zmm1, zword [0xf2d]
	vfnmadd132bf16 zmm3, zmm2, zword [0x559]
	vfnmadd132bf16 xmm14, xmm13, xmm14
	vfnmadd132bf16 ymm15, ymm8, ymm12
	vfnmadd132bf16 zmm15, zmm9, zmm15
	vfnmadd132bf16 xmm25, xmm16, xmm25
	vfnmadd132bf16 ymm29, ymm30, ymm20
	vfnmadd132bf16 zmm23, zmm23, zmm16
	vfnmadd132bf16 xmm5{k7}, xmm5, xmm6
	vfnmadd132bf16 ymm0{k3}, ymm2, yword [0xc96]
	vfnmadd132bf16 zmm0{k1}, zmm1, zmm3
	vfnmadd132bf16 xmm7{k5}{z}, xmm1, xmm5
	vfnmadd132bf16 ymm4{k6}{z}, ymm0, yword [0x950]
	vfnmadd132bf16 zmm1{k2}{z}, zmm5, zword [0x422]
	vfnmadd132bf16 xmm1, xmm7, oword [eax+1]
	vfnmadd132bf16 xmm0, xmm0, oword [eax+64]
	vfnmadd132bf16 ymm0, ymm0, yword [eax+1]
	vfnmadd132bf16 ymm5, ymm5, yword [eax+64]
	vfnmadd132bf16 zmm5, zmm2, zword [eax+1]
	vfnmadd132bf16 zmm3, zmm0, zword [eax+64]
	vfnmadd132bf16 xmm6, xmm2, [0x901]
	vfnmadd132bf16 ymm6, ymm1, [0x67f]
	vfnmadd132bf16 zmm7, zmm4, [0xe50]
