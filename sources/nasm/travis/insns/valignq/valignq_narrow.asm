	valignq xmm5, xmm4, xmm4, 0xd7
	valignq xmm5, xmm4, 0xd7
	valignq xmm3, xmm2, xmm3, 0x9c
	valignq ymm7, ymm1, ymm6, 0xa
	valignq ymm7, ymm6, 0xa
	valignq ymm6, ymm6, yword [0x82d], 0x8e
	valignq zmm1, zmm5, zword [0x194], 0x2e
	valignq zmm1, zword [0x194], 0x2e
	valignq zmm4, zmm1, zmm5, 0x78

%ifdef ERROR
	valignq xmm9, xmm14, xmm9, 0xaa
	valignq ymm14, ymm13, ymm13, 0x57
	valignq zmm15, zmm14, zmm12, 0x2e
	valignq xmm19, xmm21, xmm19, 0xa4
	valignq ymm24, ymm19, ymm29, 0x69
	valignq zmm19, zmm16, zmm16, 0xd8
%endif
