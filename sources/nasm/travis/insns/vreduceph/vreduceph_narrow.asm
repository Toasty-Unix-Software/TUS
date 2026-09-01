	vreduceph xmm6, xmm3, 0x31
	vreduceph xmm2, xmm0, 0xde
	vreduceph ymm6, ymm6, 0x2c
	vreduceph ymm3, ymm4, 0x2e
	vreduceph zmm2, zmm0, 0xce
	vreduceph zmm7, zword [0x45c], 0xf8

%ifdef ERROR
	vreduceph xmm8, xmm15, 0x6d
	vreduceph ymm13, ymm11, 0x83
	vreduceph zmm8, zmm12, 0x4c
	vreduceph xmm22, xmm28, 0x69
	vreduceph ymm23, ymm22, 0x8e
	vreduceph zmm27, zmm25, 0x9b
%endif
