default rel
	vcmpltpd xmm4, xmm1, xmm2
	vcmpltpd xmm4, xmm2
	vcmpltpd xmm2, xmm6, xmm7
	vcmpltpd ymm3, ymm5, ymm7
	vcmpltpd ymm3, ymm7
	vcmpltpd ymm3, ymm2, ymm3
	vcmpltpd k5, xmm1, oword [0x7ab]
	vcmpltpd k2, xmm2, xmm3
	vcmpltpd k4, ymm0, yword [0x3bb]
	vcmpltpd k7, ymm5, ymm6
	vcmpltpd xmm9, xmm15, xmm12
	vcmpltpd ymm12, ymm11, ymm11
	vcmpltpd k6, xmm13, xmm14
	vcmpltpd k3, ymm15, ymm14
	vcmpltpd k5{k7}, xmm0, oword [0xb6b]
	vcmpltpd k1{k7}, ymm3, ymm5
	vcmpltpd k5{k7}, zmm2, zmm1
	vcmpltpd k5, zmm7, zmm5, {sae}
	vcmpltpd xmm6, xmm3, oword [eax+1]
	vcmpltpd xmm2, xmm4, oword [eax+64]
	vcmpltpd ymm5, ymm5, yword [eax+1]
	vcmpltpd ymm1, ymm7, yword [eax+64]
	vcmpltpd k7, xmm3, oword [eax+1]
	vcmpltpd k2, xmm7, oword [eax+64]
	vcmpltpd k2, ymm2, yword [eax+1]
	vcmpltpd k6, ymm4, yword [eax+64]
	vcmpltpd k4, zmm6, zword [eax+1]
	vcmpltpd k1, zmm1, zword [eax+64]
	vcmpltpd xmm4, xmm4, [0x714]
	vcmpltpd ymm0, ymm4, [0xafd]
	vcmpltpd k2, xmm3, [0xc6c]
	vcmpltpd k2, ymm6, [0x74a]
	vcmpltpd k2, zmm0, [0xbc0]
