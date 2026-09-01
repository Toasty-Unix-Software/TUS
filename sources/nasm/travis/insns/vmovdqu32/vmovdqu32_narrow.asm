	vmovdqu32 xmm4, oword [0x905]
	vmovdqu32 xmm7, xmm0
	vmovdqu32 ymm5, ymm4
	vmovdqu32 ymm6, yword [0xf26]
	vmovdqu32 zmm3, zword [0x21e]
	vmovdqu32 zmm0, zmm4
	vmovdqu32 xmm2, xmm4
	vmovdqu32 xmm3, xmm5

%ifdef ERROR
	vmovdqu32 xmm10, xmm12
	vmovdqu32 ymm15, ymm11
	vmovdqu32 zmm12, zmm14
	vmovdqu32 xmm12, xmm11
	vmovdqu32 xmm25, xmm19
	vmovdqu32 ymm18, ymm30
	vmovdqu32 zmm31, zmm31
	vmovdqu32 xmm17, xmm26
%endif
