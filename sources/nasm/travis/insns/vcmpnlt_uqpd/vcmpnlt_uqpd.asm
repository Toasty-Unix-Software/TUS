default rel
	vcmpnlt_uqpd xmm4, xmm3, xmm1
	vcmpnlt_uqpd xmm4, xmm1
	vcmpnlt_uqpd xmm7, xmm3, xmm0
	vcmpnlt_uqpd ymm1, ymm6, ymm7
	vcmpnlt_uqpd ymm1, ymm7
	vcmpnlt_uqpd ymm4, ymm7, ymm2
	vcmpnlt_uqpd k6, xmm4, xmm1
	vcmpnlt_uqpd k6, xmm5, xmm2
	vcmpnlt_uqpd k5, ymm2, ymm4
	vcmpnlt_uqpd k4, ymm7, ymm0
	vcmpnlt_uqpd xmm10, xmm10, xmm9
	vcmpnlt_uqpd ymm12, ymm10, ymm9
	vcmpnlt_uqpd k3, xmm13, xmm12
	vcmpnlt_uqpd k2, ymm12, ymm9
	vcmpnlt_uqpd k3{k5}, xmm5, xmm0
	vcmpnlt_uqpd k7{k4}, ymm7, ymm0
	vcmpnlt_uqpd k5{k5}, zmm1, zword [0x3aa]
	vcmpnlt_uqpd k6, zmm4, zmm6, {sae}
	vcmpnlt_uqpd xmm1, xmm4, oword [eax+1]
	vcmpnlt_uqpd xmm1, xmm7, oword [eax+64]
	vcmpnlt_uqpd ymm7, ymm2, yword [eax+1]
	vcmpnlt_uqpd ymm2, ymm3, yword [eax+64]
	vcmpnlt_uqpd k2, xmm7, oword [eax+1]
	vcmpnlt_uqpd k3, xmm4, oword [eax+64]
	vcmpnlt_uqpd k4, ymm7, yword [eax+1]
	vcmpnlt_uqpd k4, ymm7, yword [eax+64]
	vcmpnlt_uqpd k7, zmm2, zword [eax+1]
	vcmpnlt_uqpd k1, zmm3, zword [eax+64]
	vcmpnlt_uqpd xmm5, xmm2, [0x55b]
	vcmpnlt_uqpd ymm5, ymm0, [0xad5]
	vcmpnlt_uqpd k6, xmm3, [0xdee]
	vcmpnlt_uqpd k7, ymm4, [0x13b]
	vcmpnlt_uqpd k7, zmm3, [0x30b]
