	vpmovw2m k2, xmm7
	vpmovw2m k7, xmm4
	vpmovw2m k6, ymm6
	vpmovw2m k6, ymm4
	vpmovw2m k3, zmm2
	vpmovw2m k1, zmm0

%ifdef ERROR
	vpmovw2m k1, xmm12
	vpmovw2m k2, ymm11
	vpmovw2m k5, zmm12
	vpmovw2m k6, xmm30
	vpmovw2m k7, ymm19
	vpmovw2m k3, zmm26
%endif
