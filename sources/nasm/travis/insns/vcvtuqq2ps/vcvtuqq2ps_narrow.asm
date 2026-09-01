	vcvtuqq2ps xmm2, xmm0
	vcvtuqq2ps xmm7, oword [0x4e1]
	vcvtuqq2ps xmm1, ymm7
	vcvtuqq2ps xmm3, ymm4
	vcvtuqq2ps ymm1, zmm0
	vcvtuqq2ps ymm0, zword [0x8ce]

%ifdef ERROR
	vcvtuqq2ps xmm10, xmm9
	vcvtuqq2ps xmm9, ymm11
	vcvtuqq2ps ymm13, zmm11
	vcvtuqq2ps xmm20, xmm31
	vcvtuqq2ps xmm21, ymm16
	vcvtuqq2ps ymm19, zmm20
%endif
