default rel
	vcmpnle_uspd xmm6, xmm4, oword [0x14a]
	vcmpnle_uspd xmm6, oword [0x14a]
	vcmpnle_uspd xmm5, xmm0, xmm0
	vcmpnle_uspd ymm5, ymm7, ymm2
	vcmpnle_uspd ymm5, ymm2
	vcmpnle_uspd ymm6, ymm3, ymm5
	vcmpnle_uspd k7, xmm3, oword [0x78d]
	vcmpnle_uspd k3, xmm1, xmm5
	vcmpnle_uspd k6, ymm4, ymm5
	vcmpnle_uspd k6, ymm0, ymm2
	vcmpnle_uspd xmm11, xmm13, xmm8
	vcmpnle_uspd ymm8, ymm9, ymm12
	vcmpnle_uspd k4, xmm14, xmm13
	vcmpnle_uspd k6, ymm13, ymm15
	vcmpnle_uspd k5{k1}, xmm7, xmm5
	vcmpnle_uspd k7{k7}, ymm0, yword [0x56e]
	vcmpnle_uspd k6{k4}, zmm4, zmm3
	vcmpnle_uspd k1, zmm2, zmm2, {sae}
	vcmpnle_uspd xmm2, xmm7, oword [eax+1]
	vcmpnle_uspd xmm4, xmm1, oword [eax+64]
	vcmpnle_uspd ymm5, ymm1, yword [eax+1]
	vcmpnle_uspd ymm6, ymm7, yword [eax+64]
	vcmpnle_uspd k5, xmm6, oword [eax+1]
	vcmpnle_uspd k6, xmm3, oword [eax+64]
	vcmpnle_uspd k6, ymm7, yword [eax+1]
	vcmpnle_uspd k7, ymm3, yword [eax+64]
	vcmpnle_uspd k4, zmm3, zword [eax+1]
	vcmpnle_uspd k4, zmm0, zword [eax+64]
	vcmpnle_uspd xmm6, xmm6, [0x1d1]
	vcmpnle_uspd ymm2, ymm7, [0x42d]
	vcmpnle_uspd k5, xmm3, [0x5aa]
	vcmpnle_uspd k4, ymm1, [0x53e]
	vcmpnle_uspd k6, zmm6, [0x492]
