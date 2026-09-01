	vpbroadcastmw2d xmm5, k4
	vpbroadcastmw2d xmm0, k4
	vpbroadcastmw2d ymm4, k5
	vpbroadcastmw2d ymm0, k1
	vpbroadcastmw2d zmm7, k4
	vpbroadcastmw2d zmm5, k7

%ifdef ERROR
	vpbroadcastmw2d xmm14, k1
	vpbroadcastmw2d ymm15, k1
	vpbroadcastmw2d zmm8, k3
	vpbroadcastmw2d xmm19, k6
	vpbroadcastmw2d ymm24, k6
	vpbroadcastmw2d zmm21, k4
%endif
