	tcvtrowps2phl zmm0, tmm2, edi
	tcvtrowps2phl zmm5, tmm2, edx
	tcvtrowps2phl zmm6, tmm0, 0x30
	tcvtrowps2phl zmm5, tmm6, 0xf

%ifdef ERROR
	tcvtrowps2phl zmm8, tmm4, r12d
	tcvtrowps2phl zmm11, tmm1, 0xde
	tcvtrowps2phl zmm20, tmm5, r16d
	tcvtrowps2phl zmm23, tmm3, 0x3b
%endif
