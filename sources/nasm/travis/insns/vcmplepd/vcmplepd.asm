default rel
	vcmplepd xmm2, xmm5, xmm4
	vcmplepd xmm2, xmm4
	vcmplepd xmm2, xmm5, xmm4
	vcmplepd ymm5, ymm0, ymm5
	vcmplepd ymm5, ymm5
	vcmplepd ymm2, ymm1, ymm1
	vcmplepd k6, xmm2, xmm4
	vcmplepd k7, xmm1, xmm2
	vcmplepd k5, ymm2, yword [0xcd8]
	vcmplepd k2, ymm5, yword [0xd69]
	vcmplepd xmm8, xmm12, xmm15
	vcmplepd ymm15, ymm9, ymm13
	vcmplepd k2, xmm14, xmm10
	vcmplepd k7, ymm9, ymm14
	vcmplepd k2{k1}, xmm2, oword [0x633]
	vcmplepd k1{k2}, ymm6, ymm7
	vcmplepd k1{k6}, zmm0, zword [0xa53]
	vcmplepd k4, zmm2, zmm4, {sae}
	vcmplepd xmm3, xmm3, oword [eax+1]
	vcmplepd xmm0, xmm1, oword [eax+64]
	vcmplepd ymm4, ymm3, yword [eax+1]
	vcmplepd ymm3, ymm7, yword [eax+64]
	vcmplepd k3, xmm0, oword [eax+1]
	vcmplepd k7, xmm7, oword [eax+64]
	vcmplepd k3, ymm6, yword [eax+1]
	vcmplepd k3, ymm6, yword [eax+64]
	vcmplepd k1, zmm0, zword [eax+1]
	vcmplepd k1, zmm0, zword [eax+64]
	vcmplepd xmm3, xmm5, [0x31a]
	vcmplepd ymm4, ymm3, [0x8b6]
	vcmplepd k1, xmm1, [0xdc4]
	vcmplepd k6, ymm2, [0x5b5]
	vcmplepd k4, zmm3, [0xe3e]
