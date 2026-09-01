default rel
	vcvtps2dq xmm0, xmm3
	vcvtps2dq xmm4, xmm1
	vcvtps2dq ymm5, yword [0x798]
	vcvtps2dq ymm1, ymm6
	vcvtps2dq xmm3, oword [0x983]
	vcvtps2dq xmm3, oword [0x298]
	vcvtps2dq ymm6, yword [0x84d]
	vcvtps2dq ymm6, ymm6
	vcvtps2dq xmm11, xmm15
	vcvtps2dq ymm14, ymm15
	vcvtps2dq xmm14, xmm10
	vcvtps2dq ymm12, ymm8
	vcvtps2dq xmm16, xmm28
	vcvtps2dq ymm31, ymm20
	vcvtps2dq xmm25, xmm31
	vcvtps2dq ymm23, ymm28
	vcvtps2dq xmm6{k5}, xmm6
	vcvtps2dq ymm7{k5}, ymm6
	vcvtps2dq zmm2{k6}, zmm6
	vcvtps2dq xmm2{k7}{z}, xmm4
	vcvtps2dq ymm2{k3}{z}, ymm4
	vcvtps2dq zmm7{k6}{z}, zword [0xfcf]
	vcvtps2dq zmm0, zmm2, {rd-sae}
	vcvtps2dq xmm7, oword [eax+1]
	vcvtps2dq xmm6, oword [eax+64]
	vcvtps2dq ymm6, yword [eax+1]
	vcvtps2dq ymm5, yword [eax+64]
	vcvtps2dq xmm3, oword [eax+1]
	vcvtps2dq xmm0, oword [eax+64]
	vcvtps2dq ymm3, yword [eax+1]
	vcvtps2dq ymm6, yword [eax+64]
	vcvtps2dq zmm5, zword [eax+1]
	vcvtps2dq zmm6, zword [eax+64]
	vcvtps2dq xmm3, [0x519]
	vcvtps2dq ymm1, [0xe52]
	vcvtps2dq xmm6, [0x1ab]
	vcvtps2dq ymm4, [0x820]
	vcvtps2dq zmm4, [0x4e9]
