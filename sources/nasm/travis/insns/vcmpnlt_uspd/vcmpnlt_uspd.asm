default rel
	vcmpnlt_uspd xmm5, xmm4, xmm1
	vcmpnlt_uspd xmm5, xmm1
	vcmpnlt_uspd xmm5, xmm4, xmm6
	vcmpnlt_uspd ymm7, ymm6, yword [0xe4b]
	vcmpnlt_uspd ymm7, yword [0xe4b]
	vcmpnlt_uspd ymm6, ymm7, ymm2
	vcmpnlt_uspd k4, xmm5, oword [0xdfe]
	vcmpnlt_uspd k7, xmm2, oword [0xc03]
	vcmpnlt_uspd k5, ymm2, ymm3
	vcmpnlt_uspd k2, ymm7, ymm4
	vcmpnlt_uspd xmm8, xmm12, xmm15
	vcmpnlt_uspd ymm10, ymm12, ymm12
	vcmpnlt_uspd k7, xmm10, xmm15
	vcmpnlt_uspd k7, ymm8, ymm8
	vcmpnlt_uspd k1{k6}, xmm6, oword [0xff2]
	vcmpnlt_uspd k5{k7}, ymm4, yword [0x8da]
	vcmpnlt_uspd k1{k4}, zmm1, zmm4
	vcmpnlt_uspd k3, zmm7, zmm4, {sae}
	vcmpnlt_uspd xmm5, xmm3, oword [eax+1]
	vcmpnlt_uspd xmm5, xmm0, oword [eax+64]
	vcmpnlt_uspd ymm2, ymm6, yword [eax+1]
	vcmpnlt_uspd ymm7, ymm2, yword [eax+64]
	vcmpnlt_uspd k5, xmm1, oword [eax+1]
	vcmpnlt_uspd k3, xmm6, oword [eax+64]
	vcmpnlt_uspd k6, ymm2, yword [eax+1]
	vcmpnlt_uspd k5, ymm0, yword [eax+64]
	vcmpnlt_uspd k1, zmm7, zword [eax+1]
	vcmpnlt_uspd k1, zmm6, zword [eax+64]
	vcmpnlt_uspd xmm2, xmm2, [0xa8f]
	vcmpnlt_uspd ymm3, ymm3, [0xf3f]
	vcmpnlt_uspd k1, xmm6, [0xa5d]
	vcmpnlt_uspd k7, ymm2, [0x581]
	vcmpnlt_uspd k6, zmm1, [0x46a]
