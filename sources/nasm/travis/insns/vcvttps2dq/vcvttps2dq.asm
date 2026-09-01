default rel
	vcvttps2dq xmm2, xmm7
	vcvttps2dq xmm4, xmm5
	vcvttps2dq ymm7, ymm4
	vcvttps2dq ymm4, ymm6
	vcvttps2dq xmm1, oword [0xcc7]
	vcvttps2dq xmm4, xmm6
	vcvttps2dq ymm6, ymm2
	vcvttps2dq ymm4, ymm2
	vcvttps2dq xmm11, xmm9
	vcvttps2dq ymm12, ymm13
	vcvttps2dq xmm13, xmm13
	vcvttps2dq ymm14, ymm12
	vcvttps2dq xmm30, xmm26
	vcvttps2dq ymm23, ymm23
	vcvttps2dq xmm21, xmm16
	vcvttps2dq ymm25, ymm27
	vcvttps2dq xmm7{k7}, xmm6
	vcvttps2dq ymm4{k7}, yword [0xaf7]
	vcvttps2dq zmm3{k1}, zword [0x669]
	vcvttps2dq xmm4{k4}{z}, xmm1
	vcvttps2dq ymm3{k4}{z}, ymm7
	vcvttps2dq zmm7{k3}{z}, zmm2
	vcvttps2dq zmm5, zmm2, {sae}
	vcvttps2dq xmm1, oword [eax+1]
	vcvttps2dq xmm0, oword [eax+64]
	vcvttps2dq ymm1, yword [eax+1]
	vcvttps2dq ymm7, yword [eax+64]
	vcvttps2dq xmm5, oword [eax+1]
	vcvttps2dq xmm4, oword [eax+64]
	vcvttps2dq ymm1, yword [eax+1]
	vcvttps2dq ymm3, yword [eax+64]
	vcvttps2dq zmm6, zword [eax+1]
	vcvttps2dq zmm2, zword [eax+64]
	vcvttps2dq xmm7, [0xd43]
	vcvttps2dq ymm5, [0x66f]
	vcvttps2dq xmm7, [0xc75]
	vcvttps2dq ymm7, [0x255]
	vcvttps2dq zmm7, [0xbb7]
