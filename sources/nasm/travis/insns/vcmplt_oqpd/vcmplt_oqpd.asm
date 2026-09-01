default rel
	vcmplt_oqpd xmm5, xmm3, xmm5
	vcmplt_oqpd xmm5, xmm5
	vcmplt_oqpd xmm0, xmm5, xmm7
	vcmplt_oqpd ymm5, ymm2, yword [0xf1b]
	vcmplt_oqpd ymm5, yword [0xf1b]
	vcmplt_oqpd ymm7, ymm5, yword [0x159]
	vcmplt_oqpd k4, xmm6, xmm3
	vcmplt_oqpd k3, xmm3, xmm5
	vcmplt_oqpd k3, ymm4, ymm3
	vcmplt_oqpd k2, ymm7, ymm3
	vcmplt_oqpd xmm14, xmm15, xmm10
	vcmplt_oqpd ymm10, ymm10, ymm15
	vcmplt_oqpd k7, xmm12, xmm13
	vcmplt_oqpd k3, ymm14, ymm8
	vcmplt_oqpd k1{k4}, xmm5, xmm6
	vcmplt_oqpd k4{k7}, ymm6, ymm4
	vcmplt_oqpd k7{k3}, zmm3, zmm5
	vcmplt_oqpd k5, zmm3, zmm1, {sae}
	vcmplt_oqpd xmm5, xmm1, oword [eax+1]
	vcmplt_oqpd xmm0, xmm2, oword [eax+64]
	vcmplt_oqpd ymm0, ymm5, yword [eax+1]
	vcmplt_oqpd ymm3, ymm7, yword [eax+64]
	vcmplt_oqpd k6, xmm0, oword [eax+1]
	vcmplt_oqpd k6, xmm5, oword [eax+64]
	vcmplt_oqpd k4, ymm0, yword [eax+1]
	vcmplt_oqpd k4, ymm2, yword [eax+64]
	vcmplt_oqpd k1, zmm5, zword [eax+1]
	vcmplt_oqpd k3, zmm6, zword [eax+64]
	vcmplt_oqpd xmm6, xmm4, [0x38c]
	vcmplt_oqpd ymm2, ymm6, [0x956]
	vcmplt_oqpd k4, xmm2, [0xddc]
	vcmplt_oqpd k4, ymm2, [0xa4f]
	vcmplt_oqpd k6, zmm4, [0x669]
