default rel
	vcvttps2udq xmm5, oword [0x390]
	vcvttps2udq xmm5, xmm3
	vcvttps2udq ymm7, ymm6
	vcvttps2udq ymm2, ymm6
	vcvttps2udq zmm6, zmm2
	vcvttps2udq zmm3, zmm7
	vcvttps2udq xmm15, xmm9
	vcvttps2udq ymm9, ymm9
	vcvttps2udq zmm13, zmm8
	vcvttps2udq xmm23, xmm30
	vcvttps2udq ymm18, ymm26
	vcvttps2udq zmm21, zmm26
	vcvttps2udq xmm3{k7}, xmm1
	vcvttps2udq ymm2{k2}, ymm2
	vcvttps2udq zmm1{k2}, zword [0x4fb]
	vcvttps2udq xmm2{k3}{z}, oword [0x183]
	vcvttps2udq ymm3{k7}{z}, ymm4
	vcvttps2udq zmm4{k6}{z}, zmm6
	vcvttps2udq zmm4, zmm2, {sae}
	vcvttps2udq xmm1, oword [eax+1]
	vcvttps2udq xmm6, oword [eax+64]
	vcvttps2udq ymm5, yword [eax+1]
	vcvttps2udq ymm0, yword [eax+64]
	vcvttps2udq zmm4, zword [eax+1]
	vcvttps2udq zmm4, zword [eax+64]
	vcvttps2udq xmm1, [0x6c9]
	vcvttps2udq ymm1, [0x6aa]
	vcvttps2udq zmm5, [0x1be]
