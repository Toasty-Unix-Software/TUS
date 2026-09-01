	vpbroadcastd xmm5, dword [0xc73]
	vpbroadcastd xmm4, dword [0xb7e]
	vpbroadcastd xmm3, xmm1
	vpbroadcastd xmm2, xmm0
	vpbroadcastd ymm7, dword [0xf53]
	vpbroadcastd ymm1, dword [0x536]
	vpbroadcastd ymm5, xmm7
	vpbroadcastd ymm6, xmm7

%ifdef ERROR
	vpbroadcastd xmm8, dword [0x80a]
	vpbroadcastd xmm8, xmm10
	vpbroadcastd ymm14, dword [0xadd]
	vpbroadcastd ymm12, xmm8
	vpbroadcastd xmm18, dword [0x8c7]
	vpbroadcastd xmm30, xmm23
	vpbroadcastd ymm18, dword [0x1d0]
	vpbroadcastd ymm25, xmm22
%endif
