	vcvttps2qq xmm2, qword [0xc06]
	vcvttps2qq xmm1, xmm7
	vcvttps2qq ymm6, xmm0
	vcvttps2qq ymm2, oword [0x32a]
	vcvttps2qq zmm0, ymm0
	vcvttps2qq zmm5, ymm5

%ifdef ERROR
	vcvttps2qq xmm14, xmm13
	vcvttps2qq ymm10, xmm11
	vcvttps2qq zmm13, ymm9
	vcvttps2qq xmm17, xmm26
	vcvttps2qq ymm18, xmm29
	vcvttps2qq zmm23, ymm27
%endif
