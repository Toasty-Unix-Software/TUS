default rel
	vcmpge_osps xmm3, xmm3, xmm6
	vcmpge_osps xmm3, xmm6
	vcmpge_osps xmm2, xmm6, xmm4
	vcmpge_osps ymm7, ymm5, yword [0x584]
	vcmpge_osps ymm7, yword [0x584]
	vcmpge_osps ymm4, ymm2, ymm0
	vcmpge_osps k4, xmm7, xmm6
	vcmpge_osps k5, xmm1, xmm4
	vcmpge_osps k1, ymm3, ymm5
	vcmpge_osps k2, ymm0, ymm7
	vcmpge_osps xmm11, xmm15, xmm8
	vcmpge_osps ymm14, ymm9, ymm9
	vcmpge_osps k6, xmm11, xmm10
	vcmpge_osps k7, ymm11, ymm12
	vcmpge_osps k1{k4}, xmm7, xmm3
	vcmpge_osps k7{k4}, ymm5, ymm5
	vcmpge_osps k3{k3}, zmm1, zmm3
	vcmpge_osps k1, zmm6, zmm2, {sae}
	vcmpge_osps xmm4, xmm3, oword [eax+1]
	vcmpge_osps xmm6, xmm0, oword [eax+64]
	vcmpge_osps ymm6, ymm3, yword [eax+1]
	vcmpge_osps ymm3, ymm2, yword [eax+64]
	vcmpge_osps k7, xmm5, oword [eax+1]
	vcmpge_osps k6, xmm7, oword [eax+64]
	vcmpge_osps k6, ymm6, yword [eax+1]
	vcmpge_osps k5, ymm4, yword [eax+64]
	vcmpge_osps k3, zmm2, zword [eax+1]
	vcmpge_osps k7, zmm3, zword [eax+64]
	vcmpge_osps xmm5, xmm7, [0x963]
	vcmpge_osps ymm4, ymm1, [0x8b6]
	vcmpge_osps k3, xmm0, [0xcbe]
	vcmpge_osps k6, ymm0, [0xb52]
	vcmpge_osps k7, zmm2, [0xc63]
