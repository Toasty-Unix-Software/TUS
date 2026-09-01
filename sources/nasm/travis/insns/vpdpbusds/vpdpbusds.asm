default rel
	vpdpbusds xmm2, xmm4, xmm1
	vpdpbusds xmm0, xmm6, xmm2
	vpdpbusds ymm5, ymm7, ymm4
	vpdpbusds ymm1, ymm5, ymm1
	vpdpbusds xmm5, xmm0, xmm3
	vpdpbusds xmm5, xmm3
	vpdpbusds xmm0, xmm0, oword [0xd7b]
	vpdpbusds ymm6, ymm6, yword [0xa0e]
	vpdpbusds ymm6, yword [0xa0e]
	vpdpbusds ymm7, ymm2, ymm0
	vpdpbusds xmm13, xmm15, xmm10
	vpdpbusds ymm9, ymm14, ymm11
	vpdpbusds xmm8, xmm9, xmm8
	vpdpbusds ymm15, ymm11, ymm10
	vpdpbusds xmm30, xmm20, xmm28
	vpdpbusds ymm24, ymm28, ymm30
	vpdpbusds xmm18, xmm18, xmm16
	vpdpbusds ymm24, ymm17, ymm18
	vpdpbusds xmm0{k6}, xmm3, xmm2
	vpdpbusds ymm7{k7}, ymm5, ymm2
	vpdpbusds zmm5{k2}, zmm5, zmm2
	vpdpbusds xmm0{k2}{z}, xmm3, xmm1
	vpdpbusds ymm3{k6}{z}, ymm3, ymm3
	vpdpbusds zmm0{k7}{z}, zmm7, zmm3
	vpdpbusds xmm5, xmm0, oword [eax+1]
	vpdpbusds xmm0, xmm7, oword [eax+64]
	vpdpbusds ymm2, ymm6, yword [eax+1]
	vpdpbusds ymm7, ymm5, yword [eax+64]
	vpdpbusds xmm2, xmm2, oword [eax+1]
	vpdpbusds xmm6, xmm1, oword [eax+64]
	vpdpbusds ymm7, ymm0, yword [eax+1]
	vpdpbusds ymm1, ymm7, yword [eax+64]
	vpdpbusds zmm5, zmm1, zword [eax+1]
	vpdpbusds zmm5, zmm4, zword [eax+64]
	vpdpbusds xmm0, xmm5, [0x4c6]
	vpdpbusds ymm7, ymm2, [0xfad]
	vpdpbusds xmm6, xmm0, [0xa8a]
	vpdpbusds ymm6, ymm3, [0xaee]
	vpdpbusds zmm3, zmm7, [0x79c]
