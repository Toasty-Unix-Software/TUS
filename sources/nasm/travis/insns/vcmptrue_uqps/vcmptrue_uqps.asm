default rel
	vcmptrue_uqps xmm7, xmm7, xmm1
	vcmptrue_uqps xmm7, xmm1
	vcmptrue_uqps xmm7, xmm2, xmm0
	vcmptrue_uqps ymm7, ymm3, ymm1
	vcmptrue_uqps ymm7, ymm1
	vcmptrue_uqps ymm3, ymm6, ymm3
	vcmptrue_uqps k3, xmm6, xmm7
	vcmptrue_uqps k3, xmm0, oword [0x56e]
	vcmptrue_uqps k6, ymm0, ymm5
	vcmptrue_uqps k3, ymm6, ymm4
	vcmptrue_uqps xmm14, xmm14, xmm11
	vcmptrue_uqps ymm14, ymm14, ymm11
	vcmptrue_uqps k4, xmm13, xmm11
	vcmptrue_uqps k1, ymm10, ymm13
	vcmptrue_uqps k7{k1}, xmm6, xmm2
	vcmptrue_uqps k7{k7}, ymm3, yword [0xe89]
	vcmptrue_uqps k6{k7}, zmm6, zword [0xef4]
	vcmptrue_uqps k5, zmm5, zmm4, {sae}
	vcmptrue_uqps xmm3, xmm2, oword [eax+1]
	vcmptrue_uqps xmm2, xmm3, oword [eax+64]
	vcmptrue_uqps ymm0, ymm1, yword [eax+1]
	vcmptrue_uqps ymm7, ymm7, yword [eax+64]
	vcmptrue_uqps k1, xmm5, oword [eax+1]
	vcmptrue_uqps k5, xmm6, oword [eax+64]
	vcmptrue_uqps k3, ymm2, yword [eax+1]
	vcmptrue_uqps k7, ymm1, yword [eax+64]
	vcmptrue_uqps k4, zmm1, zword [eax+1]
	vcmptrue_uqps k7, zmm5, zword [eax+64]
	vcmptrue_uqps xmm3, xmm1, [0x1b3]
	vcmptrue_uqps ymm7, ymm0, [0xeb3]
	vcmptrue_uqps k1, xmm4, [0xee5]
	vcmptrue_uqps k7, ymm3, [0x6ed]
	vcmptrue_uqps k6, zmm3, [0xd14]
