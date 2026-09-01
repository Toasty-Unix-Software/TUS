default rel
	vcmpnltps xmm4, xmm2, oword [0xc31]
	vcmpnltps xmm4, oword [0xc31]
	vcmpnltps xmm3, xmm5, xmm1
	vcmpnltps ymm7, ymm3, yword [0x7a4]
	vcmpnltps ymm7, yword [0x7a4]
	vcmpnltps ymm4, ymm4, yword [0xaf7]
	vcmpnltps k7, xmm4, xmm2
	vcmpnltps k5, xmm0, xmm3
	vcmpnltps k1, ymm6, yword [0xa90]
	vcmpnltps k1, ymm1, yword [0xdd0]
	vcmpnltps xmm8, xmm14, xmm12
	vcmpnltps ymm14, ymm9, ymm15
	vcmpnltps k6, xmm12, xmm9
	vcmpnltps k3, ymm11, ymm9
	vcmpnltps k7{k4}, xmm5, xmm4
	vcmpnltps k5{k2}, ymm3, ymm2
	vcmpnltps k6{k1}, zmm5, zmm6
	vcmpnltps k2, zmm7, zmm6, {sae}
	vcmpnltps xmm5, xmm2, oword [eax+1]
	vcmpnltps xmm5, xmm0, oword [eax+64]
	vcmpnltps ymm7, ymm1, yword [eax+1]
	vcmpnltps ymm4, ymm5, yword [eax+64]
	vcmpnltps k5, xmm5, oword [eax+1]
	vcmpnltps k3, xmm1, oword [eax+64]
	vcmpnltps k2, ymm6, yword [eax+1]
	vcmpnltps k4, ymm2, yword [eax+64]
	vcmpnltps k2, zmm1, zword [eax+1]
	vcmpnltps k1, zmm7, zword [eax+64]
	vcmpnltps xmm3, xmm7, [0x5fa]
	vcmpnltps ymm4, ymm3, [0xb0c]
	vcmpnltps k4, xmm1, [0x3d9]
	vcmpnltps k4, ymm4, [0xc24]
	vcmpnltps k1, zmm7, [0xbc1]
