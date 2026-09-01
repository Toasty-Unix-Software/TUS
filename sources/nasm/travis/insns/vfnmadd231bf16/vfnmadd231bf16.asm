default rel
	vfnmadd231bf16 xmm5, xmm4, xmm2
	vfnmadd231bf16 xmm4, xmm2, xmm2
	vfnmadd231bf16 ymm7, ymm2, ymm4
	vfnmadd231bf16 ymm1, ymm5, ymm3
	vfnmadd231bf16 zmm1, zmm0, zword [0x3a7]
	vfnmadd231bf16 zmm7, zmm6, zword [0x64e]
	vfnmadd231bf16 xmm9, xmm10, xmm13
	vfnmadd231bf16 ymm15, ymm13, ymm9
	vfnmadd231bf16 zmm15, zmm10, zmm15
	vfnmadd231bf16 xmm28, xmm22, xmm23
	vfnmadd231bf16 ymm29, ymm17, ymm22
	vfnmadd231bf16 zmm27, zmm25, zmm27
	vfnmadd231bf16 xmm0{k2}, xmm0, xmm2
	vfnmadd231bf16 ymm6{k7}, ymm0, ymm2
	vfnmadd231bf16 zmm7{k4}, zmm0, zmm2
	vfnmadd231bf16 xmm3{k4}{z}, xmm4, oword [0x3d5]
	vfnmadd231bf16 ymm5{k7}{z}, ymm2, yword [0x62f]
	vfnmadd231bf16 zmm1{k1}{z}, zmm7, zword [0x6bd]
	vfnmadd231bf16 xmm2, xmm3, oword [eax+1]
	vfnmadd231bf16 xmm4, xmm3, oword [eax+64]
	vfnmadd231bf16 ymm4, ymm2, yword [eax+1]
	vfnmadd231bf16 ymm0, ymm4, yword [eax+64]
	vfnmadd231bf16 zmm0, zmm6, zword [eax+1]
	vfnmadd231bf16 zmm6, zmm2, zword [eax+64]
	vfnmadd231bf16 xmm3, xmm6, [0x863]
	vfnmadd231bf16 ymm2, ymm0, [0x8c0]
	vfnmadd231bf16 zmm6, zmm4, [0x402]
