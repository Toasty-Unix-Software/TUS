	vpbroadcastmb2q xmm4, k2
	vpbroadcastmb2q xmm0, k3
	vpbroadcastmb2q ymm6, k7
	vpbroadcastmb2q ymm7, k5
	vpbroadcastmb2q zmm2, k5
	vpbroadcastmb2q zmm3, k2

%ifdef ERROR
	vpbroadcastmb2q xmm15, k5
	vpbroadcastmb2q ymm13, k2
	vpbroadcastmb2q zmm13, k3
	vpbroadcastmb2q xmm16, k3
	vpbroadcastmb2q ymm31, k1
	vpbroadcastmb2q zmm21, k1
%endif
