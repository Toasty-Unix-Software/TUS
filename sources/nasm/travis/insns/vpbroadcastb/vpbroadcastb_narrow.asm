	vpbroadcastb xmm2, byte [0x5c9]
	vpbroadcastb xmm6, byte [0x4e1]
	vpbroadcastb xmm7, xmm1
	vpbroadcastb xmm6, xmm5
	vpbroadcastb ymm6, byte [0x650]
	vpbroadcastb ymm0, byte [0x64f]
	vpbroadcastb ymm0, xmm4
	vpbroadcastb ymm3, xmm0

%ifdef ERROR
	vpbroadcastb xmm8, byte [0x51c]
	vpbroadcastb xmm9, xmm8
	vpbroadcastb ymm15, byte [0xe73]
	vpbroadcastb ymm11, xmm11
	vpbroadcastb xmm17, byte [0x9f7]
	vpbroadcastb xmm24, xmm31
	vpbroadcastb ymm26, byte [0x73a]
	vpbroadcastb ymm25, xmm21
%endif
