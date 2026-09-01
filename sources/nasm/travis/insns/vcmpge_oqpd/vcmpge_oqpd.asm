default rel
	vcmpge_oqpd xmm0, xmm5, xmm1
	vcmpge_oqpd xmm0, xmm1
	vcmpge_oqpd xmm3, xmm7, oword [0xf0a]
	vcmpge_oqpd ymm7, ymm7, ymm7
	vcmpge_oqpd ymm7, ymm7
	vcmpge_oqpd ymm7, ymm4, ymm3
	vcmpge_oqpd k4, xmm7, xmm5
	vcmpge_oqpd k1, xmm5, xmm1
	vcmpge_oqpd k1, ymm6, ymm1
	vcmpge_oqpd k7, ymm1, ymm7
	vcmpge_oqpd xmm13, xmm10, xmm15
	vcmpge_oqpd ymm15, ymm13, ymm15
	vcmpge_oqpd k7, xmm12, xmm12
	vcmpge_oqpd k1, ymm15, ymm9
	vcmpge_oqpd k7{k5}, xmm6, oword [0x5f1]
	vcmpge_oqpd k3{k4}, ymm5, yword [0x6fc]
	vcmpge_oqpd k4{k3}, zmm4, zword [0x6e4]
	vcmpge_oqpd k5, zmm1, zmm7, {sae}
	vcmpge_oqpd xmm3, xmm4, oword [eax+1]
	vcmpge_oqpd xmm1, xmm4, oword [eax+64]
	vcmpge_oqpd ymm7, ymm6, yword [eax+1]
	vcmpge_oqpd ymm2, ymm7, yword [eax+64]
	vcmpge_oqpd k2, xmm4, oword [eax+1]
	vcmpge_oqpd k1, xmm3, oword [eax+64]
	vcmpge_oqpd k7, ymm6, yword [eax+1]
	vcmpge_oqpd k4, ymm3, yword [eax+64]
	vcmpge_oqpd k5, zmm3, zword [eax+1]
	vcmpge_oqpd k6, zmm7, zword [eax+64]
	vcmpge_oqpd xmm1, xmm2, [0x37f]
	vcmpge_oqpd ymm4, ymm0, [0x9d4]
	vcmpge_oqpd k4, xmm7, [0x158]
	vcmpge_oqpd k1, ymm4, [0x9fc]
	vcmpge_oqpd k4, zmm6, [0x9b8]
