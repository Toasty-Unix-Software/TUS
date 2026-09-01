default rel
	vcmptrue_usps xmm4, xmm0, xmm3
	vcmptrue_usps xmm4, xmm3
	vcmptrue_usps xmm0, xmm5, xmm6
	vcmptrue_usps ymm6, ymm1, ymm3
	vcmptrue_usps ymm6, ymm3
	vcmptrue_usps ymm4, ymm2, yword [0xa06]
	vcmptrue_usps k1, xmm6, xmm0
	vcmptrue_usps k2, xmm7, xmm1
	vcmptrue_usps k7, ymm7, ymm0
	vcmptrue_usps k6, ymm5, ymm5
	vcmptrue_usps xmm8, xmm11, xmm15
	vcmptrue_usps ymm11, ymm12, ymm8
	vcmptrue_usps k7, xmm11, xmm12
	vcmptrue_usps k2, ymm11, ymm10
	vcmptrue_usps k2{k6}, xmm7, xmm3
	vcmptrue_usps k3{k7}, ymm0, yword [0x1e8]
	vcmptrue_usps k6{k7}, zmm7, zword [0x22c]
	vcmptrue_usps k5, zmm5, zmm6, {sae}
	vcmptrue_usps xmm1, xmm3, oword [eax+1]
	vcmptrue_usps xmm2, xmm4, oword [eax+64]
	vcmptrue_usps ymm4, ymm0, yword [eax+1]
	vcmptrue_usps ymm5, ymm0, yword [eax+64]
	vcmptrue_usps k1, xmm0, oword [eax+1]
	vcmptrue_usps k3, xmm4, oword [eax+64]
	vcmptrue_usps k2, ymm5, yword [eax+1]
	vcmptrue_usps k2, ymm0, yword [eax+64]
	vcmptrue_usps k6, zmm2, zword [eax+1]
	vcmptrue_usps k7, zmm2, zword [eax+64]
	vcmptrue_usps xmm3, xmm2, [0x55d]
	vcmptrue_usps ymm6, ymm7, [0x755]
	vcmptrue_usps k2, xmm4, [0xefd]
	vcmptrue_usps k1, ymm7, [0x6ff]
	vcmptrue_usps k3, zmm2, [0xc83]
