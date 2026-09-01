	vpmaxsw xmm5, xmm0, oword [0xd7e]
	vpmaxsw xmm5, oword [0xd7e]
	vpmaxsw xmm4, xmm4, xmm2
	vpmaxsw ymm0, ymm6, yword [0x1ac]
	vpmaxsw ymm0, yword [0x1ac]
	vpmaxsw ymm5, ymm7, ymm4
	vpmaxsw xmm7, xmm4, xmm4
	vpmaxsw xmm7, xmm4
	vpmaxsw xmm2, xmm3, oword [0x389]
	vpmaxsw ymm5, ymm0, yword [0x39b]
	vpmaxsw ymm5, yword [0x39b]
	vpmaxsw ymm3, ymm0, yword [0x9de]

%ifdef ERROR
	vpmaxsw xmm13, xmm12, xmm12
	vpmaxsw ymm10, ymm12, ymm12
	vpmaxsw xmm14, xmm13, xmm12
	vpmaxsw ymm11, ymm15, ymm15
	vpmaxsw xmm27, xmm29, xmm17
	vpmaxsw ymm23, ymm25, ymm19
	vpmaxsw xmm28, xmm18, xmm21
	vpmaxsw ymm26, ymm25, ymm19
%endif
