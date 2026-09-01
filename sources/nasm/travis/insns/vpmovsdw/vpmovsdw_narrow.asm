	vpmovsdw xmm3, xmm1
	vpmovsdw xmm0, xmm6
	vpmovsdw xmm4, ymm5
	vpmovsdw xmm4, ymm2
	vpmovsdw ymm1, zmm6
	vpmovsdw ymm5, zmm1
	vpmovsdw qword [0xc60], xmm2
	vpmovsdw qword [0x878], xmm0

%ifdef ERROR
	vpmovsdw xmm14, xmm10
	vpmovsdw xmm15, ymm10
	vpmovsdw ymm12, zmm9
	vpmovsdw qword [0xd91], xmm11
	vpmovsdw xmm23, xmm18
	vpmovsdw xmm28, ymm18
	vpmovsdw ymm23, zmm31
	vpmovsdw qword [0x57f], xmm16
%endif
