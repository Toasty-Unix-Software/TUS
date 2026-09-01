default rel
	vcmpfalse_ospd xmm5, xmm6, oword [0x554]
	vcmpfalse_ospd xmm5, oword [0x554]
	vcmpfalse_ospd xmm6, xmm3, xmm1
	vcmpfalse_ospd ymm7, ymm3, ymm3
	vcmpfalse_ospd ymm7, ymm3
	vcmpfalse_ospd ymm7, ymm6, ymm7
	vcmpfalse_ospd k2, xmm1, xmm4
	vcmpfalse_ospd k1, xmm3, xmm1
	vcmpfalse_ospd k1, ymm4, ymm1
	vcmpfalse_ospd k7, ymm7, ymm0
	vcmpfalse_ospd xmm12, xmm13, xmm9
	vcmpfalse_ospd ymm15, ymm12, ymm11
	vcmpfalse_ospd k1, xmm10, xmm10
	vcmpfalse_ospd k6, ymm12, ymm10
	vcmpfalse_ospd k4{k5}, xmm6, xmm1
	vcmpfalse_ospd k2{k7}, ymm0, ymm3
	vcmpfalse_ospd k3{k2}, zmm6, zword [0xd63]
	vcmpfalse_ospd k6, zmm1, zmm1, {sae}
	vcmpfalse_ospd xmm2, xmm1, oword [eax+1]
	vcmpfalse_ospd xmm6, xmm2, oword [eax+64]
	vcmpfalse_ospd ymm6, ymm0, yword [eax+1]
	vcmpfalse_ospd ymm0, ymm7, yword [eax+64]
	vcmpfalse_ospd k6, xmm4, oword [eax+1]
	vcmpfalse_ospd k7, xmm6, oword [eax+64]
	vcmpfalse_ospd k3, ymm0, yword [eax+1]
	vcmpfalse_ospd k3, ymm3, yword [eax+64]
	vcmpfalse_ospd k7, zmm4, zword [eax+1]
	vcmpfalse_ospd k2, zmm4, zword [eax+64]
	vcmpfalse_ospd xmm2, xmm4, [0x9e7]
	vcmpfalse_ospd ymm0, ymm6, [0xd0d]
	vcmpfalse_ospd k5, xmm1, [0x918]
	vcmpfalse_ospd k2, ymm6, [0x442]
	vcmpfalse_ospd k1, zmm2, [0x6f8]
