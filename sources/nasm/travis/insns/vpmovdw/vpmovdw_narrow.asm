	vpmovdw xmm5, xmm5
	vpmovdw xmm0, xmm3
	vpmovdw xmm1, ymm1
	vpmovdw xmm7, ymm2
	vpmovdw ymm4, zmm0
	vpmovdw ymm2, zmm7
	vpmovdw qword [0x64f], xmm1
	vpmovdw qword [0xe5e], xmm5

%ifdef ERROR
	vpmovdw xmm14, xmm15
	vpmovdw xmm12, ymm8
	vpmovdw ymm14, zmm15
	vpmovdw qword [0x397], xmm11
	vpmovdw xmm17, xmm27
	vpmovdw xmm29, ymm20
	vpmovdw ymm19, zmm31
	vpmovdw qword [0xd7d], xmm25
%endif
