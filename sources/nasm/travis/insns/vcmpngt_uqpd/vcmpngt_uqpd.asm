default rel
	vcmpngt_uqpd xmm5, xmm5, xmm0
	vcmpngt_uqpd xmm5, xmm0
	vcmpngt_uqpd xmm6, xmm7, xmm4
	vcmpngt_uqpd ymm6, ymm2, yword [0xaa0]
	vcmpngt_uqpd ymm6, yword [0xaa0]
	vcmpngt_uqpd ymm2, ymm2, ymm5
	vcmpngt_uqpd k3, xmm7, xmm4
	vcmpngt_uqpd k4, xmm4, xmm2
	vcmpngt_uqpd k1, ymm3, ymm3
	vcmpngt_uqpd k5, ymm4, yword [0xcc7]
	vcmpngt_uqpd xmm15, xmm15, xmm8
	vcmpngt_uqpd ymm10, ymm14, ymm15
	vcmpngt_uqpd k3, xmm8, xmm9
	vcmpngt_uqpd k6, ymm10, ymm15
	vcmpngt_uqpd k1{k3}, xmm3, xmm7
	vcmpngt_uqpd k1{k7}, ymm3, ymm5
	vcmpngt_uqpd k5{k2}, zmm1, zmm4
	vcmpngt_uqpd k6, zmm1, zmm0, {sae}
	vcmpngt_uqpd xmm2, xmm1, oword [eax+1]
	vcmpngt_uqpd xmm3, xmm0, oword [eax+64]
	vcmpngt_uqpd ymm0, ymm7, yword [eax+1]
	vcmpngt_uqpd ymm3, ymm3, yword [eax+64]
	vcmpngt_uqpd k3, xmm5, oword [eax+1]
	vcmpngt_uqpd k3, xmm0, oword [eax+64]
	vcmpngt_uqpd k6, ymm5, yword [eax+1]
	vcmpngt_uqpd k5, ymm6, yword [eax+64]
	vcmpngt_uqpd k6, zmm6, zword [eax+1]
	vcmpngt_uqpd k7, zmm2, zword [eax+64]
	vcmpngt_uqpd xmm1, xmm7, [0x1d0]
	vcmpngt_uqpd ymm0, ymm6, [0xff6]
	vcmpngt_uqpd k5, xmm3, [0xe78]
	vcmpngt_uqpd k5, ymm1, [0x53e]
	vcmpngt_uqpd k3, zmm1, [0xd9e]
