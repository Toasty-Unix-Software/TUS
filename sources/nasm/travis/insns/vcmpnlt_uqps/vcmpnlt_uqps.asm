default rel
	vcmpnlt_uqps xmm6, xmm5, oword [0xc56]
	vcmpnlt_uqps xmm6, oword [0xc56]
	vcmpnlt_uqps xmm5, xmm6, xmm2
	vcmpnlt_uqps ymm6, ymm4, ymm5
	vcmpnlt_uqps ymm6, ymm5
	vcmpnlt_uqps ymm5, ymm1, ymm0
	vcmpnlt_uqps k6, xmm1, xmm6
	vcmpnlt_uqps k4, xmm3, xmm7
	vcmpnlt_uqps k7, ymm4, ymm5
	vcmpnlt_uqps k2, ymm3, ymm6
	vcmpnlt_uqps xmm15, xmm8, xmm8
	vcmpnlt_uqps ymm8, ymm8, ymm9
	vcmpnlt_uqps k3, xmm14, xmm9
	vcmpnlt_uqps k2, ymm11, ymm15
	vcmpnlt_uqps k5{k2}, xmm5, xmm6
	vcmpnlt_uqps k4{k3}, ymm4, yword [0x176]
	vcmpnlt_uqps k6{k7}, zmm7, zmm1
	vcmpnlt_uqps k7, zmm5, zmm6, {sae}
	vcmpnlt_uqps xmm1, xmm1, oword [eax+1]
	vcmpnlt_uqps xmm7, xmm3, oword [eax+64]
	vcmpnlt_uqps ymm1, ymm1, yword [eax+1]
	vcmpnlt_uqps ymm4, ymm6, yword [eax+64]
	vcmpnlt_uqps k1, xmm1, oword [eax+1]
	vcmpnlt_uqps k7, xmm1, oword [eax+64]
	vcmpnlt_uqps k3, ymm5, yword [eax+1]
	vcmpnlt_uqps k4, ymm7, yword [eax+64]
	vcmpnlt_uqps k6, zmm3, zword [eax+1]
	vcmpnlt_uqps k5, zmm4, zword [eax+64]
	vcmpnlt_uqps xmm1, xmm3, [0x702]
	vcmpnlt_uqps ymm5, ymm1, [0x506]
	vcmpnlt_uqps k2, xmm6, [0x123]
	vcmpnlt_uqps k7, ymm6, [0xa7e]
	vcmpnlt_uqps k2, zmm0, [0x871]
