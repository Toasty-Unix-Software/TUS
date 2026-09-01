	vrcp14pd xmm2, xmm2
	vrcp14pd xmm2, xmm3
	vrcp14pd ymm1, yword [0xe3c]
	vrcp14pd ymm6, ymm4
	vrcp14pd zmm7, zmm0
	vrcp14pd zmm0, zword [0x2a7]

%ifdef ERROR
	vrcp14pd xmm11, xmm9
	vrcp14pd ymm12, ymm15
	vrcp14pd zmm12, zmm8
	vrcp14pd xmm17, xmm20
	vrcp14pd ymm20, ymm30
	vrcp14pd zmm26, zmm23
%endif
