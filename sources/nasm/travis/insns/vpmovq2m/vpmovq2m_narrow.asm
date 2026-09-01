	vpmovq2m k6, xmm7
	vpmovq2m k4, xmm5
	vpmovq2m k3, ymm7
	vpmovq2m k6, ymm1
	vpmovq2m k1, zmm1
	vpmovq2m k5, zmm4

%ifdef ERROR
	vpmovq2m k5, xmm8
	vpmovq2m k6, ymm11
	vpmovq2m k7, zmm13
	vpmovq2m k1, xmm19
	vpmovq2m k1, ymm24
	vpmovq2m k5, zmm20
%endif
