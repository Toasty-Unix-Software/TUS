default rel
	vpdpwusds xmm5, xmm5, oword [0x47c]
	vpdpwusds xmm5, xmm3, oword [0x283]
	vpdpwusds ymm4, ymm3, ymm1
	vpdpwusds ymm5, ymm6, ymm7
	vpdpwusds xmm1, xmm2, oword [0xd30]
	vpdpwusds xmm1, xmm5, xmm0
	vpdpwusds ymm2, ymm5, ymm3
	vpdpwusds ymm4, ymm1, ymm3
	vpdpwusds xmm11, xmm11, xmm12
	vpdpwusds ymm8, ymm10, ymm15
	vpdpwusds xmm13, xmm15, xmm15
	vpdpwusds ymm10, ymm11, ymm9
	vpdpwusds xmm18, xmm24, xmm30
	vpdpwusds ymm17, ymm22, ymm22
	vpdpwusds xmm20, xmm30, xmm23
	vpdpwusds ymm16, ymm21, ymm31
	vpdpwusds xmm2{k5}, xmm2, xmm3
	vpdpwusds ymm2{k7}, ymm2, ymm7
	vpdpwusds zmm2{k4}, zmm2, zmm5
	vpdpwusds xmm3{k5}{z}, xmm3, xmm0
	vpdpwusds ymm5{k7}{z}, ymm5, yword [0x179]
	vpdpwusds zmm3{k7}{z}, zmm1, zword [0x178]
	vpdpwusds xmm1, xmm5, oword [eax+1]
	vpdpwusds xmm7, xmm2, oword [eax+64]
	vpdpwusds ymm1, ymm3, yword [eax+1]
	vpdpwusds ymm7, ymm0, yword [eax+64]
	vpdpwusds xmm4, xmm1, oword [eax+1]
	vpdpwusds xmm6, xmm5, oword [eax+64]
	vpdpwusds ymm0, ymm2, yword [eax+1]
	vpdpwusds ymm4, ymm1, yword [eax+64]
	vpdpwusds zmm5, zmm4, zword [eax+1]
	vpdpwusds zmm0, zmm2, zword [eax+64]
	vpdpwusds xmm4, xmm2, [0xdd0]
	vpdpwusds ymm0, ymm0, [0x435]
	vpdpwusds xmm1, xmm1, [0xa90]
	vpdpwusds ymm2, ymm3, [0x475]
	vpdpwusds zmm2, zmm5, [0xac0]
