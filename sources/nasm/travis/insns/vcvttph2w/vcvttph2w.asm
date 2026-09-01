default rel
	vcvttph2w xmm1, oword [0xde1]
	vcvttph2w xmm6, oword [0x19c]
	vcvttph2w ymm5, ymm0
	vcvttph2w ymm7, ymm4
	vcvttph2w zmm2, zmm6
	vcvttph2w zmm7, zmm5
	vcvttph2w xmm11, xmm13
	vcvttph2w ymm8, ymm15
	vcvttph2w zmm13, zmm9
	vcvttph2w xmm20, xmm28
	vcvttph2w ymm20, ymm28
	vcvttph2w zmm18, zmm22
	vcvttph2w xmm6{k5}, oword [0xf90]
	vcvttph2w ymm6{k7}, yword [0x4a4]
	vcvttph2w zmm4{k7}, zmm7
	vcvttph2w xmm0{k2}{z}, xmm3
	vcvttph2w ymm7{k5}{z}, ymm2
	vcvttph2w zmm7{k6}{z}, zmm7
	vcvttph2w zmm7, zmm2, {sae}
	vcvttph2w xmm4, oword [eax+1]
	vcvttph2w xmm7, oword [eax+64]
	vcvttph2w ymm4, yword [eax+1]
	vcvttph2w ymm6, yword [eax+64]
	vcvttph2w zmm1, zword [eax+1]
	vcvttph2w zmm2, zword [eax+64]
	vcvttph2w xmm3, [0x969]
	vcvttph2w ymm5, [0x4c8]
	vcvttph2w zmm6, [0x58b]
