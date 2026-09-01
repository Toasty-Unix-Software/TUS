	vreducebf16 xmm5, xmm6, 0x88
	vreducebf16 xmm1, xmm2, 0x1f
	vreducebf16 ymm1, ymm4, 0x2f
	vreducebf16 ymm7, ymm0, 0xde
	vreducebf16 zmm3, zmm2, 0x5a
	vreducebf16 zmm3, zword [0x9f7], 0xd2

%ifdef ERROR
	vreducebf16 xmm10, xmm14, 0x8c
	vreducebf16 ymm13, ymm8, 0x86
	vreducebf16 zmm11, zmm13, 0xfa
	vreducebf16 xmm16, xmm16, 0x7a
	vreducebf16 ymm17, ymm19, 0x2
	vreducebf16 zmm22, zmm16, 0x9e
%endif
