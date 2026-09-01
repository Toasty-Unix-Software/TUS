	vcvthf82ph xmm6, xmm5
	vcvthf82ph xmm2, xmm2
	vcvthf82ph ymm2, xmm7
	vcvthf82ph ymm3, xmm6
	vcvthf82ph zmm7, ymm0
	vcvthf82ph zmm3, ymm4

%ifdef ERROR
	vcvthf82ph xmm10, xmm12
	vcvthf82ph ymm8, xmm13
	vcvthf82ph zmm14, ymm8
	vcvthf82ph xmm16, xmm28
	vcvthf82ph ymm18, xmm31
	vcvthf82ph zmm30, ymm18
%endif
