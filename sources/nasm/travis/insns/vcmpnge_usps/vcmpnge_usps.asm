default rel
	vcmpnge_usps xmm5, xmm7, xmm6
	vcmpnge_usps xmm5, xmm6
	vcmpnge_usps xmm1, xmm1, oword [0xd1c]
	vcmpnge_usps ymm7, ymm6, ymm6
	vcmpnge_usps ymm7, ymm6
	vcmpnge_usps ymm7, ymm7, ymm2
	vcmpnge_usps k7, xmm1, xmm6
	vcmpnge_usps k2, xmm3, xmm1
	vcmpnge_usps k7, ymm6, ymm5
	vcmpnge_usps k3, ymm0, ymm3
	vcmpnge_usps xmm9, xmm9, xmm12
	vcmpnge_usps ymm10, ymm8, ymm15
	vcmpnge_usps k5, xmm8, xmm13
	vcmpnge_usps k3, ymm15, ymm15
	vcmpnge_usps k3{k3}, xmm6, xmm3
	vcmpnge_usps k7{k5}, ymm6, ymm4
	vcmpnge_usps k3{k1}, zmm0, zmm2
	vcmpnge_usps k7, zmm6, zmm3, {sae}
	vcmpnge_usps xmm1, xmm5, oword [eax+1]
	vcmpnge_usps xmm4, xmm3, oword [eax+64]
	vcmpnge_usps ymm6, ymm4, yword [eax+1]
	vcmpnge_usps ymm1, ymm6, yword [eax+64]
	vcmpnge_usps k7, xmm3, oword [eax+1]
	vcmpnge_usps k5, xmm6, oword [eax+64]
	vcmpnge_usps k3, ymm1, yword [eax+1]
	vcmpnge_usps k6, ymm3, yword [eax+64]
	vcmpnge_usps k7, zmm5, zword [eax+1]
	vcmpnge_usps k7, zmm4, zword [eax+64]
	vcmpnge_usps xmm4, xmm0, [0xa93]
	vcmpnge_usps ymm4, ymm5, [0x761]
	vcmpnge_usps k7, xmm3, [0x5f2]
	vcmpnge_usps k1, ymm7, [0x6a1]
	vcmpnge_usps k5, zmm0, [0xf4d]
