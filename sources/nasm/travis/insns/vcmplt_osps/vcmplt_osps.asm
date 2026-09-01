default rel
	vcmplt_osps xmm2, xmm2, xmm0
	vcmplt_osps xmm2, xmm0
	vcmplt_osps xmm5, xmm0, xmm3
	vcmplt_osps ymm5, ymm5, ymm7
	vcmplt_osps ymm5, ymm7
	vcmplt_osps ymm1, ymm6, ymm1
	vcmplt_osps k7, xmm1, oword [0x5f8]
	vcmplt_osps k6, xmm7, xmm3
	vcmplt_osps k1, ymm3, ymm1
	vcmplt_osps k3, ymm4, yword [0x7a3]
	vcmplt_osps xmm13, xmm12, xmm11
	vcmplt_osps ymm9, ymm11, ymm10
	vcmplt_osps k2, xmm12, xmm11
	vcmplt_osps k6, ymm13, ymm8
	vcmplt_osps k1{k4}, xmm7, xmm1
	vcmplt_osps k7{k3}, ymm7, ymm2
	vcmplt_osps k7{k6}, zmm2, zmm7
	vcmplt_osps k1, zmm4, zmm0, {sae}
	vcmplt_osps xmm2, xmm1, oword [eax+1]
	vcmplt_osps xmm6, xmm5, oword [eax+64]
	vcmplt_osps ymm6, ymm0, yword [eax+1]
	vcmplt_osps ymm6, ymm0, yword [eax+64]
	vcmplt_osps k2, xmm2, oword [eax+1]
	vcmplt_osps k7, xmm0, oword [eax+64]
	vcmplt_osps k2, ymm1, yword [eax+1]
	vcmplt_osps k4, ymm6, yword [eax+64]
	vcmplt_osps k7, zmm7, zword [eax+1]
	vcmplt_osps k6, zmm6, zword [eax+64]
	vcmplt_osps xmm3, xmm6, [0x14a]
	vcmplt_osps ymm0, ymm4, [0x6ef]
	vcmplt_osps k1, xmm0, [0xa69]
	vcmplt_osps k2, ymm7, [0x72a]
	vcmplt_osps k2, zmm2, [0xed5]
