default rel
	vcmpps xmm0, xmm1, xmm1, 0x9b
	vcmpps xmm0, xmm1, 0x9b
	vcmpps xmm2, xmm1, xmm7, 0xd4
	vcmpps ymm4, ymm5, ymm5, 0xc3
	vcmpps ymm4, ymm5, 0xc3
	vcmpps ymm7, ymm4, ymm0, 0x32
	vcmpps k3, xmm3, xmm7, 0x86
	vcmpps k6, xmm3, oword [0xc8d], 0x8
	vcmpps k7, ymm0, ymm2, 0xc9
	vcmpps k5, ymm5, ymm5, 0xe7
	vcmpps xmm12, xmm12, xmm15, 0xfa
	vcmpps ymm8, ymm12, ymm8, 0x47
	vcmpps k3, xmm13, xmm13, 0x4b
	vcmpps k1, ymm13, ymm15, 0x27
	vcmpps k6{k5}, xmm5, oword [0x225], 0xbc
	vcmpps k4{k1}, ymm3, ymm7, 0x6a
	vcmpps k5{k1}, zmm5, zmm1, 0x1f
	vcmpps xmm6, xmm3, oword [eax+1], 0xd8
	vcmpps xmm4, xmm4, oword [eax+64], 0xd5
	vcmpps ymm1, ymm4, yword [eax+1], 0x50
	vcmpps ymm1, ymm7, yword [eax+64], 0xfb
	vcmpps k4, xmm0, oword [eax+1], 0xc0
	vcmpps k1, xmm6, oword [eax+64], 0xf0
	vcmpps k1, ymm1, yword [eax+1], 0xb0
	vcmpps k4, ymm2, yword [eax+64], 0xe9
	vcmpps k7, zmm0, zword [eax+1], 0x7b
	vcmpps k6, zmm5, zword [eax+64], 0xfd
	vcmpps xmm0, xmm0, [0xd3f], 0xa3
	vcmpps ymm7, ymm2, [0x930], 0x4f
	vcmpps k5, xmm0, [0x52e], 0x89
	vcmpps k1, ymm2, [0xbc8], 0xe4
	vcmpps k1, zmm7, [0x5b5], 0x48
