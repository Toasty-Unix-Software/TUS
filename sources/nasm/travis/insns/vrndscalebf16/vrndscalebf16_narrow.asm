	vrndscalebf16 xmm4, xmm1, 0x5f
	vrndscalebf16 xmm6, xmm7, 0x85
	vrndscalebf16 ymm2, yword [0xb25], 0xf2
	vrndscalebf16 ymm0, ymm1, 0xae
	vrndscalebf16 zmm7, zword [0x6ea], 0x9d
	vrndscalebf16 zmm5, zmm0, 0x6c

%ifdef ERROR
	vrndscalebf16 xmm10, xmm8, 0xb4
	vrndscalebf16 ymm8, ymm15, 0x19
	vrndscalebf16 zmm14, zmm11, 0x18
	vrndscalebf16 xmm30, xmm28, 0x25
	vrndscalebf16 ymm22, ymm16, 0x5c
	vrndscalebf16 zmm20, zmm22, 0x3
%endif
