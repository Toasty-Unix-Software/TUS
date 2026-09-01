default rel
	vpdpbssds xmm4, xmm3, oword [0xb69]
	vpdpbssds xmm7, xmm7, xmm0
	vpdpbssds ymm1, ymm6, ymm3
	vpdpbssds ymm5, ymm1, ymm5
	vpdpbssds xmm6, xmm3, oword [0xd7c]
	vpdpbssds xmm4, xmm6, xmm5
	vpdpbssds ymm7, ymm4, yword [0x6d7]
	vpdpbssds ymm3, ymm4, yword [0xaba]
	vpdpbssds xmm8, xmm12, xmm8
	vpdpbssds ymm15, ymm9, ymm9
	vpdpbssds xmm8, xmm9, xmm10
	vpdpbssds ymm10, ymm8, ymm12
	vpdpbssds xmm22, xmm22, xmm30
	vpdpbssds ymm17, ymm21, ymm27
	vpdpbssds xmm31, xmm20, xmm25
	vpdpbssds ymm21, ymm27, ymm22
	vpdpbssds xmm5{k2}, xmm4, oword [0xeba]
	vpdpbssds ymm0{k4}, ymm7, yword [0x698]
	vpdpbssds zmm5{k5}, zmm4, zmm2
	vpdpbssds xmm1{k6}{z}, xmm5, xmm7
	vpdpbssds ymm6{k3}{z}, ymm3, ymm1
	vpdpbssds zmm1{k1}{z}, zmm5, zmm6
	vpdpbssds xmm7, xmm3, oword [eax+1]
	vpdpbssds xmm2, xmm4, oword [eax+64]
	vpdpbssds ymm3, ymm2, yword [eax+1]
	vpdpbssds ymm5, ymm1, yword [eax+64]
	vpdpbssds xmm5, xmm7, oword [eax+1]
	vpdpbssds xmm3, xmm4, oword [eax+64]
	vpdpbssds ymm4, ymm3, yword [eax+1]
	vpdpbssds ymm1, ymm6, yword [eax+64]
	vpdpbssds zmm7, zmm3, zword [eax+1]
	vpdpbssds zmm4, zmm7, zword [eax+64]
	vpdpbssds xmm5, xmm3, [0x753]
	vpdpbssds ymm3, ymm3, [0x267]
	vpdpbssds xmm2, xmm7, [0x59d]
	vpdpbssds ymm7, ymm1, [0x396]
	vpdpbssds zmm4, zmm6, [0xd7a]
