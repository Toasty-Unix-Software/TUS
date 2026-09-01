default rel
	vcvttph2ibs xmm0, oword [0xb5b]
	vcvttph2ibs xmm7, oword [0x342]
	vcvttph2ibs ymm7, ymm6
	vcvttph2ibs ymm7, yword [0x4dc]
	vcvttph2ibs zmm5, zmm7
	vcvttph2ibs zmm4, zword [0xac9]
	vcvttph2ibs xmm11, xmm12
	vcvttph2ibs ymm9, ymm15
	vcvttph2ibs zmm9, zmm8
	vcvttph2ibs xmm23, xmm30
	vcvttph2ibs ymm24, ymm18
	vcvttph2ibs zmm28, zmm24
	vcvttph2ibs xmm3{k7}, oword [0x11f]
	vcvttph2ibs ymm5{k3}, ymm6
	vcvttph2ibs zmm0{k4}, zmm6
	vcvttph2ibs xmm5{k3}{z}, xmm2
	vcvttph2ibs ymm4{k3}{z}, yword [0x36e]
	vcvttph2ibs zmm3{k4}{z}, zmm6
	vcvttph2ibs zmm4, zmm4, {sae}
	vcvttph2ibs xmm2, oword [eax+1]
	vcvttph2ibs xmm0, oword [eax+64]
	vcvttph2ibs ymm4, yword [eax+1]
	vcvttph2ibs ymm6, yword [eax+64]
	vcvttph2ibs zmm1, zword [eax+1]
	vcvttph2ibs zmm4, zword [eax+64]
	vcvttph2ibs xmm7, [0x85b]
	vcvttph2ibs ymm5, [0x362]
	vcvttph2ibs zmm4, [0x659]
