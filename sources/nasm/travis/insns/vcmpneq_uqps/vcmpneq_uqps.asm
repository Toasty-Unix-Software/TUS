default rel
	vcmpneq_uqps xmm0, xmm1, xmm6
	vcmpneq_uqps xmm0, xmm6
	vcmpneq_uqps xmm3, xmm2, xmm3
	vcmpneq_uqps ymm3, ymm2, ymm6
	vcmpneq_uqps ymm3, ymm6
	vcmpneq_uqps ymm2, ymm2, yword [0xcf2]
	vcmpneq_uqps k1, xmm4, xmm6
	vcmpneq_uqps k5, xmm1, xmm4
	vcmpneq_uqps k2, ymm6, ymm0
	vcmpneq_uqps k6, ymm6, yword [0x1d0]
	vcmpneq_uqps xmm12, xmm14, xmm13
	vcmpneq_uqps ymm9, ymm11, ymm13
	vcmpneq_uqps k3, xmm12, xmm8
	vcmpneq_uqps k6, ymm13, ymm8
	vcmpneq_uqps k6{k7}, xmm5, xmm2
	vcmpneq_uqps k2{k1}, ymm2, ymm2
	vcmpneq_uqps k5{k7}, zmm7, zmm0
	vcmpneq_uqps k1, zmm6, zmm0, {sae}
	vcmpneq_uqps xmm6, xmm6, oword [eax+1]
	vcmpneq_uqps xmm6, xmm4, oword [eax+64]
	vcmpneq_uqps ymm6, ymm5, yword [eax+1]
	vcmpneq_uqps ymm0, ymm4, yword [eax+64]
	vcmpneq_uqps k4, xmm0, oword [eax+1]
	vcmpneq_uqps k5, xmm1, oword [eax+64]
	vcmpneq_uqps k5, ymm4, yword [eax+1]
	vcmpneq_uqps k7, ymm4, yword [eax+64]
	vcmpneq_uqps k5, zmm3, zword [eax+1]
	vcmpneq_uqps k1, zmm1, zword [eax+64]
	vcmpneq_uqps xmm3, xmm7, [0x142]
	vcmpneq_uqps ymm0, ymm2, [0xf8c]
	vcmpneq_uqps k2, xmm5, [0x691]
	vcmpneq_uqps k6, ymm6, [0xd0a]
	vcmpneq_uqps k3, zmm2, [0x2df]
