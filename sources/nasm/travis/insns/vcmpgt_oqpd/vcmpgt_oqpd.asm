default rel
	vcmpgt_oqpd xmm0, xmm5, oword [0x8ff]
	vcmpgt_oqpd xmm0, oword [0x8ff]
	vcmpgt_oqpd xmm1, xmm3, xmm0
	vcmpgt_oqpd ymm3, ymm1, yword [0xa53]
	vcmpgt_oqpd ymm3, yword [0xa53]
	vcmpgt_oqpd ymm3, ymm4, ymm6
	vcmpgt_oqpd k2, xmm7, xmm7
	vcmpgt_oqpd k4, xmm1, xmm1
	vcmpgt_oqpd k5, ymm3, yword [0x83c]
	vcmpgt_oqpd k3, ymm6, ymm7
	vcmpgt_oqpd xmm11, xmm13, xmm13
	vcmpgt_oqpd ymm9, ymm10, ymm10
	vcmpgt_oqpd k3, xmm15, xmm13
	vcmpgt_oqpd k4, ymm8, ymm11
	vcmpgt_oqpd k6{k4}, xmm2, oword [0x358]
	vcmpgt_oqpd k4{k6}, ymm1, ymm7
	vcmpgt_oqpd k2{k3}, zmm7, zword [0x887]
	vcmpgt_oqpd k6, zmm1, zmm3, {sae}
	vcmpgt_oqpd xmm1, xmm6, oword [eax+1]
	vcmpgt_oqpd xmm1, xmm3, oword [eax+64]
	vcmpgt_oqpd ymm6, ymm3, yword [eax+1]
	vcmpgt_oqpd ymm7, ymm0, yword [eax+64]
	vcmpgt_oqpd k3, xmm0, oword [eax+1]
	vcmpgt_oqpd k3, xmm3, oword [eax+64]
	vcmpgt_oqpd k5, ymm1, yword [eax+1]
	vcmpgt_oqpd k6, ymm1, yword [eax+64]
	vcmpgt_oqpd k6, zmm5, zword [eax+1]
	vcmpgt_oqpd k1, zmm4, zword [eax+64]
	vcmpgt_oqpd xmm1, xmm1, [0xf17]
	vcmpgt_oqpd ymm7, ymm4, [0x5b3]
	vcmpgt_oqpd k7, xmm7, [0x5ec]
	vcmpgt_oqpd k1, ymm5, [0x87c]
	vcmpgt_oqpd k1, zmm0, [0xeed]
