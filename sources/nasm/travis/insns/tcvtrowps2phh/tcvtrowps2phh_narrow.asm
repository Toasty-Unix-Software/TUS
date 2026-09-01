	tcvtrowps2phh zmm5, tmm2, edx
	tcvtrowps2phh zmm5, tmm2, ebx
	tcvtrowps2phh zmm5, tmm5, 0xda
	tcvtrowps2phh zmm0, tmm7, 0xde

%ifdef ERROR
	tcvtrowps2phh zmm8, tmm6, r12d
	tcvtrowps2phh zmm13, tmm1, 0x45
	tcvtrowps2phh zmm21, tmm7, r25d
	tcvtrowps2phh zmm29, tmm0, 0xa8
%endif
