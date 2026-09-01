	vpminsb xmm2, xmm2, xmm7
	vpminsb xmm2, xmm7
	vpminsb xmm3, xmm0, xmm7
	vpminsb ymm7, ymm6, yword [0x59b]
	vpminsb ymm7, yword [0x59b]
	vpminsb ymm0, ymm2, yword [0xb11]
	vpminsb xmm2, xmm4, oword [0xbef]
	vpminsb xmm2, oword [0xbef]
	vpminsb xmm5, xmm2, xmm0
	vpminsb ymm7, ymm1, ymm7
	vpminsb ymm7, ymm7
	vpminsb ymm7, ymm5, yword [0xa35]

%ifdef ERROR
	vpminsb xmm10, xmm11, xmm12
	vpminsb ymm9, ymm9, ymm11
	vpminsb xmm11, xmm9, xmm8
	vpminsb ymm12, ymm12, ymm14
	vpminsb xmm28, xmm23, xmm28
	vpminsb ymm22, ymm28, ymm16
	vpminsb xmm18, xmm18, xmm30
	vpminsb ymm23, ymm29, ymm27
%endif
