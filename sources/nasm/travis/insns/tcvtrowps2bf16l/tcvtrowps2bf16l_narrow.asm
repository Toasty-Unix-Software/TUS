	tcvtrowps2bf16l zmm6, tmm2, eax
	tcvtrowps2bf16l zmm2, tmm5, eax
	tcvtrowps2bf16l zmm4, tmm7, 0xf9
	tcvtrowps2bf16l zmm0, tmm2, 0xb1

%ifdef ERROR
	tcvtrowps2bf16l zmm15, tmm1, r10d
	tcvtrowps2bf16l zmm15, tmm6, 0xdc
	tcvtrowps2bf16l zmm22, tmm2, r16d
	tcvtrowps2bf16l zmm29, tmm1, 0xab
%endif
