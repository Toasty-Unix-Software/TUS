default rel
	vcmpnge_uqps xmm7, xmm1, xmm3
	vcmpnge_uqps xmm7, xmm3
	vcmpnge_uqps xmm3, xmm1, xmm0
	vcmpnge_uqps ymm1, ymm4, ymm0
	vcmpnge_uqps ymm1, ymm0
	vcmpnge_uqps ymm6, ymm1, ymm1
	vcmpnge_uqps k7, xmm6, xmm4
	vcmpnge_uqps k3, xmm1, xmm2
	vcmpnge_uqps k2, ymm7, ymm0
	vcmpnge_uqps k2, ymm0, ymm7
	vcmpnge_uqps xmm10, xmm10, xmm13
	vcmpnge_uqps ymm14, ymm11, ymm10
	vcmpnge_uqps k3, xmm9, xmm9
	vcmpnge_uqps k7, ymm12, ymm13
	vcmpnge_uqps k1{k2}, xmm0, xmm7
	vcmpnge_uqps k1{k6}, ymm1, ymm3
	vcmpnge_uqps k6{k4}, zmm6, zmm6
	vcmpnge_uqps k6, zmm7, zmm2, {sae}
	vcmpnge_uqps xmm1, xmm2, oword [eax+1]
	vcmpnge_uqps xmm5, xmm0, oword [eax+64]
	vcmpnge_uqps ymm4, ymm5, yword [eax+1]
	vcmpnge_uqps ymm6, ymm4, yword [eax+64]
	vcmpnge_uqps k6, xmm6, oword [eax+1]
	vcmpnge_uqps k7, xmm2, oword [eax+64]
	vcmpnge_uqps k7, ymm1, yword [eax+1]
	vcmpnge_uqps k6, ymm1, yword [eax+64]
	vcmpnge_uqps k7, zmm3, zword [eax+1]
	vcmpnge_uqps k5, zmm3, zword [eax+64]
	vcmpnge_uqps xmm0, xmm1, [0x4a1]
	vcmpnge_uqps ymm3, ymm2, [0xf7d]
	vcmpnge_uqps k2, xmm0, [0x1b1]
	vcmpnge_uqps k4, ymm2, [0xc67]
	vcmpnge_uqps k2, zmm0, [0xa4c]
