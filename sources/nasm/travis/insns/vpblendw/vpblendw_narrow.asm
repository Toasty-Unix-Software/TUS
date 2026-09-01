	vpblendw xmm1, xmm7, oword [0xa85], 0x26
	vpblendw xmm1, oword [0xa85], 0x26
	vpblendw xmm6, xmm0, oword [0xe7d], 0xf9
	vpblendw ymm4, ymm3, ymm5, 0xf3
	vpblendw ymm4, ymm5, 0xf3
	vpblendw ymm3, ymm6, ymm1, 0x34

%ifdef ERROR
	vpblendw xmm15, xmm12, xmm8, 0x8b
	vpblendw ymm9, ymm11, ymm15, 0xaa
%endif
