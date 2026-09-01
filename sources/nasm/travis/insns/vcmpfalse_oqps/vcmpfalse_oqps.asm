default rel
	vcmpfalse_oqps xmm0, xmm2, xmm6
	vcmpfalse_oqps xmm0, xmm6
	vcmpfalse_oqps xmm2, xmm4, xmm5
	vcmpfalse_oqps ymm1, ymm7, ymm1
	vcmpfalse_oqps ymm1, ymm1
	vcmpfalse_oqps ymm3, ymm1, ymm2
	vcmpfalse_oqps k1, xmm6, oword [0xcdb]
	vcmpfalse_oqps k1, xmm7, xmm1
	vcmpfalse_oqps k3, ymm3, yword [0x339]
	vcmpfalse_oqps k3, ymm5, yword [0x30e]
	vcmpfalse_oqps xmm15, xmm14, xmm11
	vcmpfalse_oqps ymm14, ymm11, ymm8
	vcmpfalse_oqps k2, xmm9, xmm10
	vcmpfalse_oqps k4, ymm14, ymm13
	vcmpfalse_oqps k6{k2}, xmm7, oword [0x458]
	vcmpfalse_oqps k1{k5}, ymm3, ymm6
	vcmpfalse_oqps k1{k6}, zmm6, zmm3
	vcmpfalse_oqps k6, zmm6, zmm1, {sae}
	vcmpfalse_oqps xmm5, xmm1, oword [eax+1]
	vcmpfalse_oqps xmm3, xmm0, oword [eax+64]
	vcmpfalse_oqps ymm2, ymm2, yword [eax+1]
	vcmpfalse_oqps ymm2, ymm6, yword [eax+64]
	vcmpfalse_oqps k4, xmm7, oword [eax+1]
	vcmpfalse_oqps k7, xmm6, oword [eax+64]
	vcmpfalse_oqps k6, ymm1, yword [eax+1]
	vcmpfalse_oqps k4, ymm2, yword [eax+64]
	vcmpfalse_oqps k1, zmm0, zword [eax+1]
	vcmpfalse_oqps k6, zmm6, zword [eax+64]
	vcmpfalse_oqps xmm4, xmm2, [0xb82]
	vcmpfalse_oqps ymm3, ymm3, [0xe39]
	vcmpfalse_oqps k6, xmm3, [0xeee]
	vcmpfalse_oqps k4, ymm7, [0xc8e]
	vcmpfalse_oqps k1, zmm1, [0xe80]
