default rel
	vcmpltps xmm3, xmm2, oword [0xf2c]
	vcmpltps xmm3, oword [0xf2c]
	vcmpltps xmm6, xmm4, xmm0
	vcmpltps ymm3, ymm1, ymm7
	vcmpltps ymm3, ymm7
	vcmpltps ymm2, ymm0, yword [0x9fe]
	vcmpltps k4, xmm2, xmm0
	vcmpltps k4, xmm4, xmm5
	vcmpltps k2, ymm2, ymm5
	vcmpltps k6, ymm0, ymm2
	vcmpltps xmm10, xmm13, xmm15
	vcmpltps ymm11, ymm9, ymm15
	vcmpltps k4, xmm15, xmm9
	vcmpltps k4, ymm13, ymm15
	vcmpltps k5{k1}, xmm3, xmm6
	vcmpltps k6{k4}, ymm7, ymm6
	vcmpltps k7{k3}, zmm0, zmm2
	vcmpltps k5, zmm7, zmm3, {sae}
	vcmpltps xmm0, xmm1, oword [eax+1]
	vcmpltps xmm6, xmm1, oword [eax+64]
	vcmpltps ymm4, ymm3, yword [eax+1]
	vcmpltps ymm7, ymm5, yword [eax+64]
	vcmpltps k7, xmm1, oword [eax+1]
	vcmpltps k1, xmm2, oword [eax+64]
	vcmpltps k6, ymm1, yword [eax+1]
	vcmpltps k1, ymm6, yword [eax+64]
	vcmpltps k2, zmm0, zword [eax+1]
	vcmpltps k6, zmm6, zword [eax+64]
	vcmpltps xmm4, xmm6, [0x878]
	vcmpltps ymm7, ymm3, [0x9d9]
	vcmpltps k6, xmm7, [0x566]
	vcmpltps k5, ymm5, [0x13a]
	vcmpltps k3, zmm0, [0x3a9]
