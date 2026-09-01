	valignd xmm2, xmm5, oword [0xa65], 0x52
	valignd xmm2, oword [0xa65], 0x52
	valignd xmm4, xmm6, xmm2, 0xcd
	valignd ymm3, ymm0, ymm5, 0x61
	valignd ymm3, ymm5, 0x61
	valignd ymm2, ymm2, ymm2, 0xc5
	valignd zmm7, zmm1, zmm4, 0xdf
	valignd zmm7, zmm4, 0xdf
	valignd zmm4, zmm2, zmm5, 0x80

%ifdef ERROR
	valignd xmm9, xmm8, xmm14, 0xd0
	valignd ymm13, ymm13, ymm9, 0x2c
	valignd zmm13, zmm11, zmm13, 0x5d
	valignd xmm26, xmm26, xmm28, 0xdb
	valignd ymm25, ymm23, ymm18, 0x91
	valignd zmm25, zmm25, zmm29, 0xd5
%endif
