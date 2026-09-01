default rel
	vcmpord_qpd xmm7, xmm1, xmm4
	vcmpord_qpd xmm7, xmm4
	vcmpord_qpd xmm7, xmm4, xmm4
	vcmpord_qpd ymm7, ymm7, ymm7
	vcmpord_qpd ymm7, ymm7
	vcmpord_qpd ymm4, ymm7, ymm2
	vcmpord_qpd k1, xmm0, oword [0x9b2]
	vcmpord_qpd k4, xmm7, oword [0x731]
	vcmpord_qpd k2, ymm4, ymm6
	vcmpord_qpd k2, ymm3, yword [0x146]
	vcmpord_qpd xmm15, xmm14, xmm13
	vcmpord_qpd ymm15, ymm13, ymm11
	vcmpord_qpd k4, xmm8, xmm9
	vcmpord_qpd k3, ymm14, ymm9
	vcmpord_qpd k4{k4}, xmm5, xmm0
	vcmpord_qpd k6{k2}, ymm2, ymm1
	vcmpord_qpd k5{k4}, zmm5, zmm2
	vcmpord_qpd k7, zmm1, zmm5, {sae}
	vcmpord_qpd xmm0, xmm6, oword [eax+1]
	vcmpord_qpd xmm1, xmm6, oword [eax+64]
	vcmpord_qpd ymm6, ymm0, yword [eax+1]
	vcmpord_qpd ymm1, ymm1, yword [eax+64]
	vcmpord_qpd k5, xmm1, oword [eax+1]
	vcmpord_qpd k5, xmm3, oword [eax+64]
	vcmpord_qpd k7, ymm3, yword [eax+1]
	vcmpord_qpd k4, ymm1, yword [eax+64]
	vcmpord_qpd k2, zmm7, zword [eax+1]
	vcmpord_qpd k4, zmm3, zword [eax+64]
	vcmpord_qpd xmm3, xmm7, [0x173]
	vcmpord_qpd ymm7, ymm7, [0x283]
	vcmpord_qpd k4, xmm7, [0xfd4]
	vcmpord_qpd k5, ymm4, [0x527]
	vcmpord_qpd k6, zmm7, [0x44d]
