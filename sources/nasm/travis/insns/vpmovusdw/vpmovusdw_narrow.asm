	vpmovusdw xmm3, xmm4
	vpmovusdw xmm3, xmm1
	vpmovusdw xmm3, ymm4
	vpmovusdw xmm3, ymm4
	vpmovusdw ymm2, zmm6
	vpmovusdw ymm1, zmm4
	vpmovusdw qword [0x356], xmm6
	vpmovusdw qword [0x7f0], xmm4

%ifdef ERROR
	vpmovusdw xmm15, xmm12
	vpmovusdw xmm14, ymm8
	vpmovusdw ymm11, zmm9
	vpmovusdw qword [0xeb4], xmm11
	vpmovusdw xmm22, xmm17
	vpmovusdw xmm26, ymm17
	vpmovusdw ymm20, zmm23
	vpmovusdw qword [0xd94], xmm21
%endif
