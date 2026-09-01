	vpmovqw xmm5, xmm2
	vpmovqw xmm1, xmm3
	vpmovqw xmm2, ymm3
	vpmovqw xmm2, ymm4
	vpmovqw xmm4, zmm7
	vpmovqw xmm5, zmm1
	vpmovqw dword [0xeb3], xmm7
	vpmovqw dword [0x585], xmm6

%ifdef ERROR
	vpmovqw xmm13, xmm9
	vpmovqw xmm8, ymm10
	vpmovqw xmm14, zmm15
	vpmovqw dword [0x860], xmm8
	vpmovqw xmm27, xmm22
	vpmovqw xmm31, ymm30
	vpmovqw xmm16, zmm23
	vpmovqw dword [0x148], xmm16
%endif
