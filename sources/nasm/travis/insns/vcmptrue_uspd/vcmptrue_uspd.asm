default rel
	vcmptrue_uspd xmm7, xmm1, xmm2
	vcmptrue_uspd xmm7, xmm2
	vcmptrue_uspd xmm7, xmm0, xmm5
	vcmptrue_uspd ymm4, ymm7, yword [0xa12]
	vcmptrue_uspd ymm4, yword [0xa12]
	vcmptrue_uspd ymm7, ymm0, yword [0xb13]
	vcmptrue_uspd k6, xmm1, oword [0x467]
	vcmptrue_uspd k6, xmm2, xmm7
	vcmptrue_uspd k4, ymm2, yword [0x655]
	vcmptrue_uspd k5, ymm3, yword [0xbfd]
	vcmptrue_uspd xmm9, xmm14, xmm10
	vcmptrue_uspd ymm9, ymm13, ymm15
	vcmptrue_uspd k7, xmm12, xmm11
	vcmptrue_uspd k1, ymm12, ymm12
	vcmptrue_uspd k5{k5}, xmm5, xmm3
	vcmptrue_uspd k5{k3}, ymm1, yword [0x527]
	vcmptrue_uspd k2{k4}, zmm1, zword [0xcfa]
	vcmptrue_uspd k7, zmm3, zmm0, {sae}
	vcmptrue_uspd xmm7, xmm0, oword [eax+1]
	vcmptrue_uspd xmm3, xmm6, oword [eax+64]
	vcmptrue_uspd ymm5, ymm4, yword [eax+1]
	vcmptrue_uspd ymm7, ymm1, yword [eax+64]
	vcmptrue_uspd k1, xmm1, oword [eax+1]
	vcmptrue_uspd k1, xmm6, oword [eax+64]
	vcmptrue_uspd k4, ymm0, yword [eax+1]
	vcmptrue_uspd k2, ymm7, yword [eax+64]
	vcmptrue_uspd k1, zmm1, zword [eax+1]
	vcmptrue_uspd k5, zmm3, zword [eax+64]
	vcmptrue_uspd xmm6, xmm2, [0x319]
	vcmptrue_uspd ymm0, ymm5, [0xf12]
	vcmptrue_uspd k2, xmm7, [0xfa3]
	vcmptrue_uspd k2, ymm4, [0x95d]
	vcmptrue_uspd k4, zmm5, [0x69f]
