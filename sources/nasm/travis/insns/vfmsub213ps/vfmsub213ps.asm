default rel
	vfmsub213ps xmm2, xmm4, xmm7
	vfmsub213ps xmm3, xmm2, xmm2
	vfmsub213ps ymm4, ymm2, ymm0
	vfmsub213ps ymm1, ymm4, ymm6
	vfmsub213ps xmm2, xmm1, oword [0xf87]
	vfmsub213ps xmm3, xmm4, xmm4
	vfmsub213ps ymm4, ymm1, ymm3
	vfmsub213ps ymm2, ymm1, yword [0x2dd]
	vfmsub213ps xmm15, xmm11, xmm14
	vfmsub213ps ymm15, ymm12, ymm10
	vfmsub213ps xmm9, xmm14, xmm9
	vfmsub213ps ymm12, ymm12, ymm9
	vfmsub213ps xmm29, xmm20, xmm21
	vfmsub213ps ymm17, ymm27, ymm30
	vfmsub213ps xmm30, xmm20, xmm18
	vfmsub213ps ymm20, ymm25, ymm19
	vfmsub213ps xmm6{k7}, xmm7, xmm5
	vfmsub213ps ymm0{k2}, ymm6, ymm4
	vfmsub213ps zmm7{k7}, zmm1, zmm6
	vfmsub213ps xmm7{k4}{z}, xmm4, xmm7
	vfmsub213ps ymm7{k3}{z}, ymm4, ymm3
	vfmsub213ps zmm6{k5}{z}, zmm1, zword [0x1af]
	vfmsub213ps zmm2, zmm3, zmm7, {rn-sae}
	vfmsub213ps xmm3, xmm5, oword [eax+1]
	vfmsub213ps xmm3, xmm4, oword [eax+64]
	vfmsub213ps ymm1, ymm4, yword [eax+1]
	vfmsub213ps ymm1, ymm3, yword [eax+64]
	vfmsub213ps xmm6, xmm1, oword [eax+1]
	vfmsub213ps xmm1, xmm0, oword [eax+64]
	vfmsub213ps ymm4, ymm5, yword [eax+1]
	vfmsub213ps ymm5, ymm6, yword [eax+64]
	vfmsub213ps zmm4, zmm7, zword [eax+1]
	vfmsub213ps zmm3, zmm1, zword [eax+64]
	vfmsub213ps xmm2, xmm7, [0x525]
	vfmsub213ps ymm1, ymm1, [0x181]
	vfmsub213ps xmm3, xmm6, [0x314]
	vfmsub213ps ymm1, ymm3, [0x21a]
	vfmsub213ps zmm0, zmm0, [0x3f4]
