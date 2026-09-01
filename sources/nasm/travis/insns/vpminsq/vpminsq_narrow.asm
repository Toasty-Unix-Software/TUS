	vpminsq xmm4, xmm1, xmm1
	vpminsq xmm4, xmm1
	vpminsq xmm4, xmm5, xmm6
	vpminsq ymm6, ymm0, yword [0x89b]
	vpminsq ymm6, yword [0x89b]
	vpminsq ymm3, ymm4, ymm1
	vpminsq zmm2, zmm6, zword [0x61c]
	vpminsq zmm2, zword [0x61c]
	vpminsq zmm2, zmm4, zmm1

%ifdef ERROR
	vpminsq xmm13, xmm14, xmm12
	vpminsq ymm13, ymm15, ymm13
	vpminsq zmm9, zmm13, zmm12
	vpminsq xmm18, xmm17, xmm28
	vpminsq ymm21, ymm21, ymm20
	vpminsq zmm29, zmm17, zmm19
%endif
