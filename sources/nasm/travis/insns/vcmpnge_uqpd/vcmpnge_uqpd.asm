default rel
	vcmpnge_uqpd xmm2, xmm4, oword [0x87b]
	vcmpnge_uqpd xmm2, oword [0x87b]
	vcmpnge_uqpd xmm6, xmm3, xmm6
	vcmpnge_uqpd ymm2, ymm5, yword [0x73e]
	vcmpnge_uqpd ymm2, yword [0x73e]
	vcmpnge_uqpd ymm0, ymm3, yword [0xaf9]
	vcmpnge_uqpd k2, xmm7, xmm2
	vcmpnge_uqpd k4, xmm1, oword [0x6cc]
	vcmpnge_uqpd k2, ymm2, yword [0xa67]
	vcmpnge_uqpd k7, ymm2, ymm3
	vcmpnge_uqpd xmm11, xmm12, xmm15
	vcmpnge_uqpd ymm11, ymm11, ymm10
	vcmpnge_uqpd k1, xmm8, xmm10
	vcmpnge_uqpd k1, ymm10, ymm14
	vcmpnge_uqpd k6{k4}, xmm1, xmm1
	vcmpnge_uqpd k5{k7}, ymm7, ymm5
	vcmpnge_uqpd k2{k3}, zmm7, zmm0
	vcmpnge_uqpd k3, zmm2, zmm2, {sae}
	vcmpnge_uqpd xmm3, xmm5, oword [eax+1]
	vcmpnge_uqpd xmm7, xmm4, oword [eax+64]
	vcmpnge_uqpd ymm1, ymm7, yword [eax+1]
	vcmpnge_uqpd ymm0, ymm6, yword [eax+64]
	vcmpnge_uqpd k3, xmm6, oword [eax+1]
	vcmpnge_uqpd k4, xmm5, oword [eax+64]
	vcmpnge_uqpd k1, ymm0, yword [eax+1]
	vcmpnge_uqpd k2, ymm4, yword [eax+64]
	vcmpnge_uqpd k4, zmm7, zword [eax+1]
	vcmpnge_uqpd k7, zmm1, zword [eax+64]
	vcmpnge_uqpd xmm0, xmm0, [0x56b]
	vcmpnge_uqpd ymm6, ymm4, [0x3da]
	vcmpnge_uqpd k6, xmm1, [0x91a]
	vcmpnge_uqpd k5, ymm2, [0xbd9]
	vcmpnge_uqpd k1, zmm5, [0x41e]
