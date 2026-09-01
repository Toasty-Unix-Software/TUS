default rel
	vpdpwuuds xmm1, xmm6, oword [0xb5d]
	vpdpwuuds xmm5, xmm3, xmm7
	vpdpwuuds ymm1, ymm0, ymm7
	vpdpwuuds ymm4, ymm3, ymm6
	vpdpwuuds xmm0, xmm3, xmm6
	vpdpwuuds xmm1, xmm7, xmm4
	vpdpwuuds ymm7, ymm7, yword [0xee9]
	vpdpwuuds ymm4, ymm5, ymm1
	vpdpwuuds xmm11, xmm15, xmm15
	vpdpwuuds ymm11, ymm12, ymm12
	vpdpwuuds xmm13, xmm13, xmm12
	vpdpwuuds ymm8, ymm14, ymm9
	vpdpwuuds xmm30, xmm22, xmm28
	vpdpwuuds ymm17, ymm17, ymm24
	vpdpwuuds xmm16, xmm26, xmm21
	vpdpwuuds ymm29, ymm17, ymm24
	vpdpwuuds xmm4{k5}, xmm2, xmm6
	vpdpwuuds ymm3{k6}, ymm3, yword [0x3fa]
	vpdpwuuds zmm5{k7}, zmm7, zmm5
	vpdpwuuds xmm2{k2}{z}, xmm4, xmm7
	vpdpwuuds ymm4{k3}{z}, ymm5, ymm4
	vpdpwuuds zmm5{k4}{z}, zmm5, zmm2
	vpdpwuuds xmm7, xmm7, oword [eax+1]
	vpdpwuuds xmm6, xmm1, oword [eax+64]
	vpdpwuuds ymm6, ymm2, yword [eax+1]
	vpdpwuuds ymm7, ymm7, yword [eax+64]
	vpdpwuuds xmm7, xmm0, oword [eax+1]
	vpdpwuuds xmm1, xmm5, oword [eax+64]
	vpdpwuuds ymm3, ymm2, yword [eax+1]
	vpdpwuuds ymm2, ymm5, yword [eax+64]
	vpdpwuuds zmm2, zmm1, zword [eax+1]
	vpdpwuuds zmm2, zmm4, zword [eax+64]
	vpdpwuuds xmm7, xmm4, [0x5d3]
	vpdpwuuds ymm0, ymm0, [0xb53]
	vpdpwuuds xmm2, xmm2, [0xd93]
	vpdpwuuds ymm6, ymm0, [0x2d5]
	vpdpwuuds zmm3, zmm1, [0x503]
