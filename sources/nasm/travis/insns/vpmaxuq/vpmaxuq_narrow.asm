	vpmaxuq xmm4, xmm0, oword [0xcf5]
	vpmaxuq xmm4, oword [0xcf5]
	vpmaxuq xmm5, xmm3, xmm5
	vpmaxuq ymm5, ymm6, yword [0x46d]
	vpmaxuq ymm5, yword [0x46d]
	vpmaxuq ymm7, ymm3, ymm1
	vpmaxuq zmm7, zmm0, zmm6
	vpmaxuq zmm7, zmm6
	vpmaxuq zmm3, zmm6, zmm3

%ifdef ERROR
	vpmaxuq xmm9, xmm11, xmm12
	vpmaxuq ymm11, ymm9, ymm12
	vpmaxuq zmm13, zmm15, zmm15
	vpmaxuq xmm19, xmm29, xmm23
	vpmaxuq ymm20, ymm29, ymm22
	vpmaxuq zmm18, zmm16, zmm20
%endif
