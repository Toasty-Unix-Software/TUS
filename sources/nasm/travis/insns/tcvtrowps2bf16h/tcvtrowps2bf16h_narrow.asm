	tcvtrowps2bf16h zmm4, tmm4, ecx
	tcvtrowps2bf16h zmm2, tmm1, esi
	tcvtrowps2bf16h zmm3, tmm0, 0xc7
	tcvtrowps2bf16h zmm4, tmm1, 0x9

%ifdef ERROR
	tcvtrowps2bf16h zmm10, tmm5, r9d
	tcvtrowps2bf16h zmm15, tmm6, 0x87
	tcvtrowps2bf16h zmm30, tmm2, r21d
	tcvtrowps2bf16h zmm27, tmm0, 0xf1
%endif
