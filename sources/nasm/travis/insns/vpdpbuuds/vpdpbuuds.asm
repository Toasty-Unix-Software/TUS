default rel
	vpdpbuuds xmm1, xmm6, oword [0x7c8]
	vpdpbuuds xmm0, xmm2, xmm5
	vpdpbuuds ymm5, ymm3, ymm5
	vpdpbuuds ymm4, ymm1, ymm2
	vpdpbuuds xmm1, xmm1, xmm4
	vpdpbuuds xmm1, xmm4, xmm3
	vpdpbuuds ymm3, ymm2, ymm0
	vpdpbuuds ymm4, ymm2, yword [0xd42]
	vpdpbuuds xmm10, xmm15, xmm8
	vpdpbuuds ymm14, ymm11, ymm15
	vpdpbuuds xmm14, xmm9, xmm15
	vpdpbuuds ymm14, ymm8, ymm9
	vpdpbuuds xmm31, xmm25, xmm27
	vpdpbuuds ymm23, ymm18, ymm24
	vpdpbuuds xmm24, xmm19, xmm27
	vpdpbuuds ymm24, ymm21, ymm30
	vpdpbuuds xmm0{k7}, xmm1, xmm5
	vpdpbuuds ymm5{k3}, ymm2, ymm2
	vpdpbuuds zmm3{k1}, zmm5, zmm5
	vpdpbuuds xmm5{k2}{z}, xmm4, xmm5
	vpdpbuuds ymm3{k6}{z}, ymm0, ymm1
	vpdpbuuds zmm3{k3}{z}, zmm4, zmm2
	vpdpbuuds xmm3, xmm2, oword [eax+1]
	vpdpbuuds xmm6, xmm7, oword [eax+64]
	vpdpbuuds ymm6, ymm5, yword [eax+1]
	vpdpbuuds ymm7, ymm7, yword [eax+64]
	vpdpbuuds xmm4, xmm1, oword [eax+1]
	vpdpbuuds xmm4, xmm4, oword [eax+64]
	vpdpbuuds ymm4, ymm2, yword [eax+1]
	vpdpbuuds ymm7, ymm6, yword [eax+64]
	vpdpbuuds zmm2, zmm6, zword [eax+1]
	vpdpbuuds zmm0, zmm0, zword [eax+64]
	vpdpbuuds xmm1, xmm5, [0xbd0]
	vpdpbuuds ymm6, ymm7, [0x673]
	vpdpbuuds xmm3, xmm2, [0x927]
	vpdpbuuds ymm7, ymm5, [0x30a]
	vpdpbuuds zmm6, zmm4, [0xb96]
