default rel
	vcmpgepd xmm1, xmm0, xmm1
	vcmpgepd xmm1, xmm1
	vcmpgepd xmm7, xmm0, oword [0xa23]
	vcmpgepd ymm7, ymm7, ymm7
	vcmpgepd ymm7, ymm7
	vcmpgepd ymm3, ymm3, yword [0xb07]
	vcmpgepd k4, xmm6, xmm6
	vcmpgepd k4, xmm2, xmm6
	vcmpgepd k1, ymm4, yword [0xc65]
	vcmpgepd k4, ymm1, ymm2
	vcmpgepd xmm9, xmm10, xmm15
	vcmpgepd ymm11, ymm8, ymm11
	vcmpgepd k5, xmm8, xmm8
	vcmpgepd k5, ymm15, ymm15
	vcmpgepd k1{k1}, xmm6, oword [0x371]
	vcmpgepd k4{k7}, ymm2, yword [0x5ab]
	vcmpgepd k5{k5}, zmm6, zword [0xebd]
	vcmpgepd k2, zmm5, zmm7, {sae}
	vcmpgepd xmm7, xmm2, oword [eax+1]
	vcmpgepd xmm1, xmm7, oword [eax+64]
	vcmpgepd ymm1, ymm0, yword [eax+1]
	vcmpgepd ymm1, ymm7, yword [eax+64]
	vcmpgepd k3, xmm7, oword [eax+1]
	vcmpgepd k7, xmm4, oword [eax+64]
	vcmpgepd k7, ymm1, yword [eax+1]
	vcmpgepd k1, ymm4, yword [eax+64]
	vcmpgepd k5, zmm6, zword [eax+1]
	vcmpgepd k1, zmm0, zword [eax+64]
	vcmpgepd xmm2, xmm3, [0x253]
	vcmpgepd ymm3, ymm5, [0xd4e]
	vcmpgepd k3, xmm1, [0xaa9]
	vcmpgepd k4, ymm2, [0xc21]
	vcmpgepd k2, zmm2, [0xd11]
