default rel
	vcmpfalse_oqpd xmm7, xmm4, xmm5
	vcmpfalse_oqpd xmm7, xmm5
	vcmpfalse_oqpd xmm5, xmm0, xmm1
	vcmpfalse_oqpd ymm3, ymm6, ymm5
	vcmpfalse_oqpd ymm3, ymm5
	vcmpfalse_oqpd ymm3, ymm7, ymm4
	vcmpfalse_oqpd k5, xmm2, xmm1
	vcmpfalse_oqpd k5, xmm5, xmm4
	vcmpfalse_oqpd k6, ymm2, yword [0xc83]
	vcmpfalse_oqpd k3, ymm0, ymm7
	vcmpfalse_oqpd xmm11, xmm9, xmm10
	vcmpfalse_oqpd ymm8, ymm13, ymm11
	vcmpfalse_oqpd k4, xmm12, xmm15
	vcmpfalse_oqpd k5, ymm8, ymm11
	vcmpfalse_oqpd k6{k4}, xmm4, xmm4
	vcmpfalse_oqpd k6{k2}, ymm3, ymm6
	vcmpfalse_oqpd k4{k4}, zmm0, zmm1
	vcmpfalse_oqpd k3, zmm4, zmm1, {sae}
	vcmpfalse_oqpd xmm1, xmm5, oword [eax+1]
	vcmpfalse_oqpd xmm3, xmm6, oword [eax+64]
	vcmpfalse_oqpd ymm5, ymm4, yword [eax+1]
	vcmpfalse_oqpd ymm1, ymm6, yword [eax+64]
	vcmpfalse_oqpd k4, xmm5, oword [eax+1]
	vcmpfalse_oqpd k6, xmm3, oword [eax+64]
	vcmpfalse_oqpd k1, ymm4, yword [eax+1]
	vcmpfalse_oqpd k6, ymm5, yword [eax+64]
	vcmpfalse_oqpd k3, zmm7, zword [eax+1]
	vcmpfalse_oqpd k2, zmm0, zword [eax+64]
	vcmpfalse_oqpd xmm7, xmm2, [0x63b]
	vcmpfalse_oqpd ymm7, ymm7, [0x878]
	vcmpfalse_oqpd k4, xmm7, [0x405]
	vcmpfalse_oqpd k2, ymm0, [0x3ab]
	vcmpfalse_oqpd k6, zmm6, [0x2cf]
