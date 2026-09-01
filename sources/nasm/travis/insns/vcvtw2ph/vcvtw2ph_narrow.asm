	vcvtw2ph xmm4, xmm6
	vcvtw2ph xmm3, oword [0xcc1]
	vcvtw2ph ymm0, ymm5
	vcvtw2ph ymm7, ymm7
	vcvtw2ph zmm1, zmm3
	vcvtw2ph zmm2, zword [0xe49]

%ifdef ERROR
	vcvtw2ph xmm13, xmm9
	vcvtw2ph ymm11, ymm14
	vcvtw2ph zmm13, zmm11
	vcvtw2ph xmm17, xmm17
	vcvtw2ph ymm17, ymm16
	vcvtw2ph zmm20, zmm28
%endif
