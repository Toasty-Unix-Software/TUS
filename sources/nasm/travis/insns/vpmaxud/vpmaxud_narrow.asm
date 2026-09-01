	vpmaxud xmm7, xmm4, oword [0x967]
	vpmaxud xmm7, oword [0x967]
	vpmaxud xmm5, xmm1, xmm5
	vpmaxud ymm6, ymm1, ymm6
	vpmaxud ymm6, ymm6
	vpmaxud ymm5, ymm6, ymm7
	vpmaxud xmm2, xmm1, xmm3
	vpmaxud xmm2, xmm3
	vpmaxud xmm1, xmm3, xmm4
	vpmaxud ymm6, ymm6, ymm1
	vpmaxud ymm6, ymm1
	vpmaxud ymm5, ymm7, yword [0x28d]

%ifdef ERROR
	vpmaxud xmm8, xmm13, xmm14
	vpmaxud ymm12, ymm15, ymm9
	vpmaxud xmm14, xmm9, xmm15
	vpmaxud ymm11, ymm12, ymm9
	vpmaxud xmm27, xmm19, xmm31
	vpmaxud ymm22, ymm28, ymm30
	vpmaxud xmm23, xmm20, xmm26
	vpmaxud ymm26, ymm30, ymm19
%endif
