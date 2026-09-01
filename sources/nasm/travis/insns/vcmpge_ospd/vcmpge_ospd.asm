default rel
	vcmpge_ospd xmm1, xmm4, xmm1
	vcmpge_ospd xmm1, xmm1
	vcmpge_ospd xmm0, xmm4, xmm4
	vcmpge_ospd ymm7, ymm3, ymm5
	vcmpge_ospd ymm7, ymm5
	vcmpge_ospd ymm7, ymm2, yword [0x30b]
	vcmpge_ospd k7, xmm4, xmm4
	vcmpge_ospd k3, xmm6, xmm6
	vcmpge_ospd k1, ymm0, ymm2
	vcmpge_ospd k4, ymm7, ymm1
	vcmpge_ospd xmm10, xmm11, xmm10
	vcmpge_ospd ymm11, ymm10, ymm13
	vcmpge_ospd k5, xmm13, xmm15
	vcmpge_ospd k6, ymm8, ymm8
	vcmpge_ospd k6{k1}, xmm1, xmm2
	vcmpge_ospd k2{k1}, ymm6, ymm0
	vcmpge_ospd k6{k5}, zmm7, zmm1
	vcmpge_ospd k2, zmm1, zmm0, {sae}
	vcmpge_ospd xmm7, xmm0, oword [eax+1]
	vcmpge_ospd xmm2, xmm3, oword [eax+64]
	vcmpge_ospd ymm5, ymm7, yword [eax+1]
	vcmpge_ospd ymm2, ymm4, yword [eax+64]
	vcmpge_ospd k7, xmm7, oword [eax+1]
	vcmpge_ospd k7, xmm2, oword [eax+64]
	vcmpge_ospd k2, ymm7, yword [eax+1]
	vcmpge_ospd k3, ymm0, yword [eax+64]
	vcmpge_ospd k2, zmm3, zword [eax+1]
	vcmpge_ospd k3, zmm7, zword [eax+64]
	vcmpge_ospd xmm0, xmm3, [0x382]
	vcmpge_ospd ymm7, ymm0, [0xb65]
	vcmpge_ospd k3, xmm6, [0xa84]
	vcmpge_ospd k2, ymm1, [0x5fc]
	vcmpge_ospd k5, zmm5, [0xe3c]
