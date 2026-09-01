	vpmovb2m k7, xmm4
	vpmovb2m k7, xmm0
	vpmovb2m k6, ymm7
	vpmovb2m k4, ymm4
	vpmovb2m k1, zmm6
	vpmovb2m k1, zmm0

%ifdef ERROR
	vpmovb2m k7, xmm11
	vpmovb2m k2, ymm14
	vpmovb2m k3, zmm14
	vpmovb2m k1, xmm27
	vpmovb2m k5, ymm19
	vpmovb2m k5, zmm20
%endif
