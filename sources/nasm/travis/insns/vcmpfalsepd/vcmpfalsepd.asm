default rel
	vcmpfalsepd xmm6, xmm7, xmm6
	vcmpfalsepd xmm6, xmm6
	vcmpfalsepd xmm5, xmm4, xmm6
	vcmpfalsepd ymm4, ymm1, ymm1
	vcmpfalsepd ymm4, ymm1
	vcmpfalsepd ymm4, ymm7, ymm3
	vcmpfalsepd k4, xmm4, xmm7
	vcmpfalsepd k4, xmm3, oword [0x864]
	vcmpfalsepd k2, ymm7, ymm5
	vcmpfalsepd k1, ymm0, yword [0x1f6]
	vcmpfalsepd xmm10, xmm15, xmm11
	vcmpfalsepd ymm10, ymm13, ymm12
	vcmpfalsepd k1, xmm12, xmm11
	vcmpfalsepd k6, ymm12, ymm8
	vcmpfalsepd k4{k7}, xmm0, xmm7
	vcmpfalsepd k5{k6}, ymm3, ymm5
	vcmpfalsepd k6{k1}, zmm7, zmm4
	vcmpfalsepd k4, zmm6, zmm6, {sae}
	vcmpfalsepd xmm5, xmm2, oword [eax+1]
	vcmpfalsepd xmm4, xmm5, oword [eax+64]
	vcmpfalsepd ymm0, ymm2, yword [eax+1]
	vcmpfalsepd ymm7, ymm0, yword [eax+64]
	vcmpfalsepd k2, xmm7, oword [eax+1]
	vcmpfalsepd k6, xmm3, oword [eax+64]
	vcmpfalsepd k3, ymm5, yword [eax+1]
	vcmpfalsepd k5, ymm2, yword [eax+64]
	vcmpfalsepd k4, zmm4, zword [eax+1]
	vcmpfalsepd k3, zmm5, zword [eax+64]
	vcmpfalsepd xmm4, xmm2, [0x81b]
	vcmpfalsepd ymm2, ymm6, [0x567]
	vcmpfalsepd k3, xmm4, [0x62f]
	vcmpfalsepd k3, ymm5, [0xe76]
	vcmpfalsepd k6, zmm3, [0x8c7]
