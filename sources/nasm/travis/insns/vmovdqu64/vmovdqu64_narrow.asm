	vmovdqu64 xmm7, xmm2
	vmovdqu64 xmm4, xmm3
	vmovdqu64 ymm7, yword [0x251]
	vmovdqu64 ymm2, ymm5
	vmovdqu64 zmm0, zmm6
	vmovdqu64 zmm2, zmm6
	vmovdqu64 oword [0xecd], xmm0
	vmovdqu64 xmm6, xmm5

%ifdef ERROR
	vmovdqu64 xmm8, xmm11
	vmovdqu64 ymm12, ymm10
	vmovdqu64 zmm12, zmm12
	vmovdqu64 xmm13, xmm12
	vmovdqu64 xmm27, xmm26
	vmovdqu64 ymm21, ymm25
	vmovdqu64 zmm31, zmm28
	vmovdqu64 xmm19, xmm20
%endif
