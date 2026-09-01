default rel
	vcmpgtps xmm3, xmm4, xmm6
	vcmpgtps xmm3, xmm6
	vcmpgtps xmm1, xmm4, xmm2
	vcmpgtps ymm1, ymm4, yword [0x2a9]
	vcmpgtps ymm1, yword [0x2a9]
	vcmpgtps ymm2, ymm4, yword [0x700]
	vcmpgtps k2, xmm0, oword [0x3b6]
	vcmpgtps k2, xmm7, xmm4
	vcmpgtps k2, ymm1, ymm2
	vcmpgtps k2, ymm6, ymm5
	vcmpgtps xmm10, xmm14, xmm14
	vcmpgtps ymm8, ymm15, ymm14
	vcmpgtps k5, xmm14, xmm10
	vcmpgtps k2, ymm13, ymm9
	vcmpgtps k3{k5}, xmm3, xmm3
	vcmpgtps k7{k5}, ymm4, yword [0x367]
	vcmpgtps k1{k7}, zmm4, zmm2
	vcmpgtps k1, zmm3, zmm5, {sae}
	vcmpgtps xmm0, xmm3, oword [eax+1]
	vcmpgtps xmm6, xmm7, oword [eax+64]
	vcmpgtps ymm4, ymm7, yword [eax+1]
	vcmpgtps ymm7, ymm6, yword [eax+64]
	vcmpgtps k5, xmm4, oword [eax+1]
	vcmpgtps k2, xmm3, oword [eax+64]
	vcmpgtps k6, ymm3, yword [eax+1]
	vcmpgtps k2, ymm1, yword [eax+64]
	vcmpgtps k7, zmm2, zword [eax+1]
	vcmpgtps k5, zmm4, zword [eax+64]
	vcmpgtps xmm6, xmm2, [0x2bf]
	vcmpgtps ymm2, ymm1, [0x9c6]
	vcmpgtps k1, xmm6, [0xad0]
	vcmpgtps k7, ymm0, [0x8b4]
	vcmpgtps k7, zmm6, [0xf5e]
