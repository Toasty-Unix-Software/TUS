	tcvtrowd2ps zmm1, tmm0, ecx
	tcvtrowd2ps zmm2, tmm3, ebp
	tcvtrowd2ps zmm2, tmm3, 0xbf
	tcvtrowd2ps zmm6, tmm1, 0x97

%ifdef ERROR
	tcvtrowd2ps zmm10, tmm7, r8d
	tcvtrowd2ps zmm8, tmm6, 0xca
	tcvtrowd2ps zmm28, tmm3, r28d
	tcvtrowd2ps zmm24, tmm1, 0x4a
%endif
