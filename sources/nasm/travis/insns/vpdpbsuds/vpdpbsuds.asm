default rel
	vpdpbsuds xmm0, xmm5, xmm2
	vpdpbsuds xmm3, xmm0, oword [0xc06]
	vpdpbsuds ymm2, ymm3, yword [0x418]
	vpdpbsuds ymm0, ymm1, ymm4
	vpdpbsuds xmm6, xmm7, xmm4
	vpdpbsuds xmm5, xmm2, xmm5
	vpdpbsuds ymm5, ymm5, ymm0
	vpdpbsuds ymm1, ymm3, ymm7
	vpdpbsuds xmm15, xmm14, xmm15
	vpdpbsuds ymm11, ymm10, ymm13
	vpdpbsuds xmm13, xmm15, xmm8
	vpdpbsuds ymm15, ymm11, ymm15
	vpdpbsuds xmm22, xmm26, xmm24
	vpdpbsuds ymm21, ymm18, ymm25
	vpdpbsuds xmm22, xmm29, xmm27
	vpdpbsuds ymm23, ymm20, ymm22
	vpdpbsuds xmm7{k1}, xmm7, xmm3
	vpdpbsuds ymm6{k5}, ymm0, ymm7
	vpdpbsuds zmm4{k3}, zmm6, zmm5
	vpdpbsuds xmm7{k4}{z}, xmm3, xmm4
	vpdpbsuds ymm1{k5}{z}, ymm2, ymm6
	vpdpbsuds zmm7{k3}{z}, zmm1, zword [0x96b]
	vpdpbsuds xmm1, xmm6, oword [eax+1]
	vpdpbsuds xmm4, xmm4, oword [eax+64]
	vpdpbsuds ymm4, ymm2, yword [eax+1]
	vpdpbsuds ymm3, ymm6, yword [eax+64]
	vpdpbsuds xmm5, xmm6, oword [eax+1]
	vpdpbsuds xmm6, xmm0, oword [eax+64]
	vpdpbsuds ymm6, ymm1, yword [eax+1]
	vpdpbsuds ymm6, ymm2, yword [eax+64]
	vpdpbsuds zmm2, zmm0, zword [eax+1]
	vpdpbsuds zmm3, zmm3, zword [eax+64]
	vpdpbsuds xmm3, xmm0, [0xfa8]
	vpdpbsuds ymm7, ymm1, [0x33a]
	vpdpbsuds xmm6, xmm1, [0x5c0]
	vpdpbsuds ymm7, ymm4, [0xf94]
	vpdpbsuds zmm3, zmm2, [0x761]
