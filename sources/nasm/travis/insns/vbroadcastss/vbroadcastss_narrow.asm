	vbroadcastss xmm3, dword [0x832]
	vbroadcastss xmm0, dword [0xa59]
	vbroadcastss ymm7, dword [0x7cc]
	vbroadcastss ymm7, dword [0x99f]
	vbroadcastss xmm1, xmm6
	vbroadcastss xmm7, xmm2
	vbroadcastss ymm4, xmm4
	vbroadcastss ymm5, xmm2

%ifdef ERROR
	vbroadcastss xmm13, dword [0xfad]
	vbroadcastss ymm8, dword [0x771]
	vbroadcastss xmm15, xmm8
	vbroadcastss ymm12, xmm12
	vbroadcastss xmm22, dword [0x33d]
	vbroadcastss ymm20, dword [0x994]
	vbroadcastss xmm26, xmm17
	vbroadcastss ymm25, xmm21
%endif
