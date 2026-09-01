	vcvtps2ibs xmm4, xmm7
	vcvtps2ibs xmm2, oword [0xb55]
	vcvtps2ibs ymm7, yword [0xe1d]
	vcvtps2ibs ymm0, ymm5
	vcvtps2ibs zmm2, zmm0
	vcvtps2ibs zmm6, zmm4

%ifdef ERROR
	vcvtps2ibs xmm9, xmm9
	vcvtps2ibs ymm12, ymm11
	vcvtps2ibs zmm8, zmm11
	vcvtps2ibs xmm31, xmm25
	vcvtps2ibs ymm29, ymm25
	vcvtps2ibs zmm16, zmm27
%endif
