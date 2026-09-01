	vcvtps2qq xmm7, qword [0xcac]
	vcvtps2qq xmm5, xmm0
	vcvtps2qq ymm6, xmm1
	vcvtps2qq ymm6, xmm1
	vcvtps2qq zmm2, ymm3
	vcvtps2qq zmm5, ymm0

%ifdef ERROR
	vcvtps2qq xmm14, xmm10
	vcvtps2qq ymm15, xmm8
	vcvtps2qq zmm8, ymm12
	vcvtps2qq xmm18, xmm17
	vcvtps2qq ymm30, xmm24
	vcvtps2qq zmm16, ymm26
%endif
