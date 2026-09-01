default rel
	vcvtps2udq xmm4, xmm2
	vcvtps2udq xmm2, xmm7
	vcvtps2udq ymm3, ymm1
	vcvtps2udq ymm5, ymm0
	vcvtps2udq zmm7, zmm6
	vcvtps2udq zmm7, zword [0x5bb]
	vcvtps2udq xmm10, xmm14
	vcvtps2udq ymm8, ymm13
	vcvtps2udq zmm15, zmm14
	vcvtps2udq xmm26, xmm18
	vcvtps2udq ymm28, ymm17
	vcvtps2udq zmm21, zmm22
	vcvtps2udq xmm1{k4}, xmm5
	vcvtps2udq ymm1{k4}, ymm5
	vcvtps2udq zmm3{k5}, zword [0x3da]
	vcvtps2udq xmm7{k1}{z}, xmm0
	vcvtps2udq ymm1{k3}{z}, ymm1
	vcvtps2udq zmm1{k2}{z}, zmm6
	vcvtps2udq zmm5, zmm7, {rz-sae}
	vcvtps2udq xmm2, oword [eax+1]
	vcvtps2udq xmm6, oword [eax+64]
	vcvtps2udq ymm5, yword [eax+1]
	vcvtps2udq ymm7, yword [eax+64]
	vcvtps2udq zmm0, zword [eax+1]
	vcvtps2udq zmm2, zword [eax+64]
	vcvtps2udq xmm0, [0xe2a]
	vcvtps2udq ymm2, [0x205]
	vcvtps2udq zmm4, [0xa9f]
