default rel
	vcmpneq_uspd xmm4, xmm7, oword [0xbec]
	vcmpneq_uspd xmm4, oword [0xbec]
	vcmpneq_uspd xmm5, xmm0, xmm3
	vcmpneq_uspd ymm7, ymm2, ymm5
	vcmpneq_uspd ymm7, ymm5
	vcmpneq_uspd ymm6, ymm0, ymm1
	vcmpneq_uspd k3, xmm5, oword [0x34d]
	vcmpneq_uspd k3, xmm0, xmm3
	vcmpneq_uspd k1, ymm2, yword [0xadd]
	vcmpneq_uspd k4, ymm6, ymm0
	vcmpneq_uspd xmm13, xmm12, xmm10
	vcmpneq_uspd ymm8, ymm10, ymm11
	vcmpneq_uspd k4, xmm15, xmm8
	vcmpneq_uspd k7, ymm14, ymm10
	vcmpneq_uspd k7{k5}, xmm7, xmm5
	vcmpneq_uspd k6{k5}, ymm6, yword [0xd98]
	vcmpneq_uspd k7{k5}, zmm0, zmm7
	vcmpneq_uspd k2, zmm1, zmm5, {sae}
	vcmpneq_uspd xmm0, xmm1, oword [eax+1]
	vcmpneq_uspd xmm1, xmm7, oword [eax+64]
	vcmpneq_uspd ymm1, ymm0, yword [eax+1]
	vcmpneq_uspd ymm1, ymm6, yword [eax+64]
	vcmpneq_uspd k7, xmm1, oword [eax+1]
	vcmpneq_uspd k5, xmm5, oword [eax+64]
	vcmpneq_uspd k3, ymm7, yword [eax+1]
	vcmpneq_uspd k5, ymm5, yword [eax+64]
	vcmpneq_uspd k3, zmm6, zword [eax+1]
	vcmpneq_uspd k6, zmm7, zword [eax+64]
	vcmpneq_uspd xmm6, xmm6, [0x19c]
	vcmpneq_uspd ymm3, ymm0, [0x5cb]
	vcmpneq_uspd k6, xmm7, [0xf73]
	vcmpneq_uspd k3, ymm2, [0x54e]
	vcmpneq_uspd k6, zmm4, [0x7dd]
