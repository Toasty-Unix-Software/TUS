	vcvttps2udq xmm5, oword [0x390]
	vcvttps2udq xmm5, xmm3
	vcvttps2udq ymm7, ymm6
	vcvttps2udq ymm2, ymm6
	vcvttps2udq zmm6, zmm2
	vcvttps2udq zmm3, zmm7

%ifdef ERROR
	vcvttps2udq xmm15, xmm9
	vcvttps2udq ymm9, ymm9
	vcvttps2udq zmm13, zmm8
	vcvttps2udq xmm23, xmm30
	vcvttps2udq ymm18, ymm26
	vcvttps2udq zmm21, zmm26
%endif
