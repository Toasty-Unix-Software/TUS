default rel
	vcmpnltpd xmm5, xmm6, xmm6
	vcmpnltpd xmm5, xmm6
	vcmpnltpd xmm3, xmm3, xmm2
	vcmpnltpd ymm0, ymm4, ymm3
	vcmpnltpd ymm0, ymm3
	vcmpnltpd ymm6, ymm0, yword [0xf26]
	vcmpnltpd k5, xmm3, oword [0xa05]
	vcmpnltpd k3, xmm0, xmm4
	vcmpnltpd k4, ymm7, ymm1
	vcmpnltpd k1, ymm1, yword [0x633]
	vcmpnltpd xmm9, xmm13, xmm9
	vcmpnltpd ymm13, ymm14, ymm13
	vcmpnltpd k2, xmm15, xmm13
	vcmpnltpd k7, ymm12, ymm15
	vcmpnltpd k1{k5}, xmm0, oword [0x464]
	vcmpnltpd k1{k1}, ymm7, ymm5
	vcmpnltpd k7{k2}, zmm1, zword [0xdc4]
	vcmpnltpd k4, zmm1, zmm5, {sae}
	vcmpnltpd xmm4, xmm2, oword [eax+1]
	vcmpnltpd xmm0, xmm4, oword [eax+64]
	vcmpnltpd ymm6, ymm0, yword [eax+1]
	vcmpnltpd ymm5, ymm0, yword [eax+64]
	vcmpnltpd k7, xmm1, oword [eax+1]
	vcmpnltpd k1, xmm7, oword [eax+64]
	vcmpnltpd k5, ymm6, yword [eax+1]
	vcmpnltpd k2, ymm3, yword [eax+64]
	vcmpnltpd k7, zmm1, zword [eax+1]
	vcmpnltpd k3, zmm3, zword [eax+64]
	vcmpnltpd xmm1, xmm6, [0xbca]
	vcmpnltpd ymm3, ymm2, [0xb72]
	vcmpnltpd k5, xmm0, [0x9ac]
	vcmpnltpd k2, ymm0, [0x720]
	vcmpnltpd k5, zmm5, [0xc3f]
