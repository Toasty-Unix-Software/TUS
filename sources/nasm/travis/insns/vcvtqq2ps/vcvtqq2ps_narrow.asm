	vcvtqq2ps xmm1, xmm6
	vcvtqq2ps xmm0, xmm3
	vcvtqq2ps xmm4, ymm1
	vcvtqq2ps xmm2, yword [0xb08]
	vcvtqq2ps ymm0, zmm2
	vcvtqq2ps ymm7, zmm0

%ifdef ERROR
	vcvtqq2ps xmm13, xmm15
	vcvtqq2ps xmm10, ymm10
	vcvtqq2ps ymm12, zmm13
	vcvtqq2ps xmm27, xmm21
	vcvtqq2ps xmm20, ymm29
	vcvtqq2ps ymm29, zmm26
%endif
