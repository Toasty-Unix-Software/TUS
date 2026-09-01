	vpmaxsd xmm5, xmm5, xmm4
	vpmaxsd xmm5, xmm4
	vpmaxsd xmm7, xmm6, xmm3
	vpmaxsd ymm3, ymm6, ymm1
	vpmaxsd ymm3, ymm1
	vpmaxsd ymm6, ymm5, ymm0
	vpmaxsd xmm0, xmm5, xmm4
	vpmaxsd xmm0, xmm4
	vpmaxsd xmm3, xmm3, xmm2
	vpmaxsd ymm6, ymm3, yword [0x2b9]
	vpmaxsd ymm6, yword [0x2b9]
	vpmaxsd ymm2, ymm1, yword [0xd84]

%ifdef ERROR
	vpmaxsd xmm11, xmm14, xmm14
	vpmaxsd ymm8, ymm14, ymm12
	vpmaxsd xmm10, xmm14, xmm9
	vpmaxsd ymm11, ymm14, ymm15
	vpmaxsd xmm27, xmm19, xmm22
	vpmaxsd ymm24, ymm18, ymm20
	vpmaxsd xmm21, xmm26, xmm21
	vpmaxsd ymm18, ymm30, ymm17
%endif
