	tilemovrow zmm6, tmm4, 0x6e
	tilemovrow zmm1, tmm3, 0x92
	tilemovrow zmm4, tmm5, edx
	tilemovrow zmm5, tmm7, edi

%ifdef ERROR
	tilemovrow zmm15, tmm0, 0xaa
	tilemovrow zmm14, tmm5, r10d
	tilemovrow zmm17, tmm2, 0xb7
	tilemovrow zmm18, tmm3, r16d
%endif
