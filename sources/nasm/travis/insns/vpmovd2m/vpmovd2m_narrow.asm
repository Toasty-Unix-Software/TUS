	vpmovd2m k4, xmm2
	vpmovd2m k4, xmm1
	vpmovd2m k7, ymm1
	vpmovd2m k4, ymm0
	vpmovd2m k5, zmm2
	vpmovd2m k2, zmm6

%ifdef ERROR
	vpmovd2m k6, xmm9
	vpmovd2m k6, ymm10
	vpmovd2m k1, zmm10
	vpmovd2m k3, xmm26
	vpmovd2m k7, ymm26
	vpmovd2m k5, zmm16
%endif
