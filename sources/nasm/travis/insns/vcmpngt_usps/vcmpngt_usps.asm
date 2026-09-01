default rel
	vcmpngt_usps xmm6, xmm1, oword [0xea7]
	vcmpngt_usps xmm6, oword [0xea7]
	vcmpngt_usps xmm2, xmm4, oword [0x15a]
	vcmpngt_usps ymm4, ymm1, ymm1
	vcmpngt_usps ymm4, ymm1
	vcmpngt_usps ymm6, ymm7, ymm4
	vcmpngt_usps k1, xmm2, xmm2
	vcmpngt_usps k2, xmm0, xmm3
	vcmpngt_usps k7, ymm4, ymm6
	vcmpngt_usps k5, ymm2, ymm5
	vcmpngt_usps xmm15, xmm10, xmm11
	vcmpngt_usps ymm9, ymm10, ymm15
	vcmpngt_usps k2, xmm11, xmm8
	vcmpngt_usps k5, ymm12, ymm14
	vcmpngt_usps k3{k1}, xmm0, xmm1
	vcmpngt_usps k5{k2}, ymm2, ymm3
	vcmpngt_usps k1{k6}, zmm5, zmm0
	vcmpngt_usps k6, zmm1, zmm5, {sae}
	vcmpngt_usps xmm2, xmm5, oword [eax+1]
	vcmpngt_usps xmm7, xmm7, oword [eax+64]
	vcmpngt_usps ymm6, ymm7, yword [eax+1]
	vcmpngt_usps ymm2, ymm7, yword [eax+64]
	vcmpngt_usps k7, xmm1, oword [eax+1]
	vcmpngt_usps k5, xmm5, oword [eax+64]
	vcmpngt_usps k1, ymm2, yword [eax+1]
	vcmpngt_usps k4, ymm4, yword [eax+64]
	vcmpngt_usps k5, zmm7, zword [eax+1]
	vcmpngt_usps k3, zmm2, zword [eax+64]
	vcmpngt_usps xmm7, xmm6, [0x32c]
	vcmpngt_usps ymm6, ymm6, [0x4ea]
	vcmpngt_usps k2, xmm6, [0xd84]
	vcmpngt_usps k7, ymm5, [0xe98]
	vcmpngt_usps k6, zmm6, [0x846]
