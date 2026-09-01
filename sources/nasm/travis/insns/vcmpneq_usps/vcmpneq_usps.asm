default rel
	vcmpneq_usps xmm7, xmm3, xmm6
	vcmpneq_usps xmm7, xmm6
	vcmpneq_usps xmm0, xmm4, oword [0x7d0]
	vcmpneq_usps ymm5, ymm2, ymm6
	vcmpneq_usps ymm5, ymm6
	vcmpneq_usps ymm2, ymm2, ymm4
	vcmpneq_usps k7, xmm3, xmm6
	vcmpneq_usps k3, xmm1, oword [0xb1e]
	vcmpneq_usps k3, ymm1, ymm4
	vcmpneq_usps k2, ymm4, ymm0
	vcmpneq_usps xmm14, xmm12, xmm12
	vcmpneq_usps ymm12, ymm13, ymm14
	vcmpneq_usps k7, xmm14, xmm15
	vcmpneq_usps k1, ymm15, ymm11
	vcmpneq_usps k4{k2}, xmm5, xmm1
	vcmpneq_usps k5{k7}, ymm5, ymm6
	vcmpneq_usps k6{k2}, zmm1, zword [0x649]
	vcmpneq_usps k2, zmm6, zmm2, {sae}
	vcmpneq_usps xmm7, xmm1, oword [eax+1]
	vcmpneq_usps xmm0, xmm0, oword [eax+64]
	vcmpneq_usps ymm7, ymm7, yword [eax+1]
	vcmpneq_usps ymm1, ymm4, yword [eax+64]
	vcmpneq_usps k5, xmm7, oword [eax+1]
	vcmpneq_usps k2, xmm3, oword [eax+64]
	vcmpneq_usps k4, ymm6, yword [eax+1]
	vcmpneq_usps k4, ymm5, yword [eax+64]
	vcmpneq_usps k7, zmm4, zword [eax+1]
	vcmpneq_usps k4, zmm3, zword [eax+64]
	vcmpneq_usps xmm0, xmm7, [0xf9d]
	vcmpneq_usps ymm1, ymm1, [0x3a7]
	vcmpneq_usps k6, xmm1, [0xa66]
	vcmpneq_usps k2, ymm1, [0x48f]
	vcmpneq_usps k3, zmm1, [0xf0b]
