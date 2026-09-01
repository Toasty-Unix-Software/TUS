default rel
	vcmpngtpd xmm0, xmm4, xmm6
	vcmpngtpd xmm0, xmm6
	vcmpngtpd xmm2, xmm1, xmm5
	vcmpngtpd ymm4, ymm7, yword [0x2c3]
	vcmpngtpd ymm4, yword [0x2c3]
	vcmpngtpd ymm0, ymm7, ymm3
	vcmpngtpd k4, xmm6, xmm7
	vcmpngtpd k3, xmm6, xmm4
	vcmpngtpd k1, ymm6, ymm2
	vcmpngtpd k4, ymm5, ymm3
	vcmpngtpd xmm13, xmm14, xmm14
	vcmpngtpd ymm10, ymm14, ymm14
	vcmpngtpd k1, xmm15, xmm12
	vcmpngtpd k4, ymm14, ymm9
	vcmpngtpd k6{k3}, xmm6, xmm7
	vcmpngtpd k5{k3}, ymm3, yword [0x825]
	vcmpngtpd k5{k7}, zmm4, zmm4
	vcmpngtpd k5, zmm1, zmm3, {sae}
	vcmpngtpd xmm0, xmm4, oword [eax+1]
	vcmpngtpd xmm1, xmm1, oword [eax+64]
	vcmpngtpd ymm6, ymm5, yword [eax+1]
	vcmpngtpd ymm6, ymm7, yword [eax+64]
	vcmpngtpd k2, xmm6, oword [eax+1]
	vcmpngtpd k3, xmm1, oword [eax+64]
	vcmpngtpd k6, ymm1, yword [eax+1]
	vcmpngtpd k4, ymm6, yword [eax+64]
	vcmpngtpd k1, zmm2, zword [eax+1]
	vcmpngtpd k3, zmm3, zword [eax+64]
	vcmpngtpd xmm4, xmm0, [0x5d5]
	vcmpngtpd ymm1, ymm1, [0x22b]
	vcmpngtpd k4, xmm2, [0x822]
	vcmpngtpd k1, ymm2, [0x597]
	vcmpngtpd k2, zmm0, [0xf05]
