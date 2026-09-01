default rel
	vcvtps2phx xmm5, oword [0xb67]
	vcvtps2phx xmm6, xmm4
	vcvtps2phx xmm4, yword [0x163]
	vcvtps2phx xmm6, yword [0xb3c]
	vcvtps2phx ymm2, zword [0x5a2]
	vcvtps2phx ymm4, zword [0x3e9]
	vcvtps2phx xmm9, xmm8
	vcvtps2phx xmm13, ymm15
	vcvtps2phx ymm14, zmm12
	vcvtps2phx xmm24, xmm24
	vcvtps2phx xmm31, ymm28
	vcvtps2phx ymm16, zmm31
	vcvtps2phx xmm0{k1}, xmm0
	vcvtps2phx xmm7{k6}, yword [0x513]
	vcvtps2phx ymm6{k3}, zword [0x452]
	vcvtps2phx xmm2{k2}{z}, xmm5
	vcvtps2phx xmm7{k6}{z}, yword [0xad5]
	vcvtps2phx ymm5{k6}{z}, zmm1
	vcvtps2phx ymm5, zmm6, {ru-sae}
	vcvtps2phx xmm1, oword [eax+1]
	vcvtps2phx xmm7, oword [eax+64]
	vcvtps2phx xmm5, yword [eax+1]
	vcvtps2phx xmm5, yword [eax+64]
	vcvtps2phx ymm0, zword [eax+1]
	vcvtps2phx ymm7, zword [eax+64]
	vcvtps2phx xmm0, [0xdc5]
	vcvtps2phx xmm3, [0x1d7]
	vcvtps2phx ymm5, [0xb3b]
