	vmovdqa64 xmm0, oword [0x2ed]
	vmovdqa64 xmm4, xmm3
	vmovdqa64 ymm0, yword [0xc2e]
	vmovdqa64 ymm4, ymm2
	vmovdqa64 zmm0, zmm2
	vmovdqa64 zmm2, zword [0x72f]
	vmovdqa64 xmm6, xmm3
	vmovdqa64 xmm3, xmm5

%ifdef ERROR
	vmovdqa64 xmm8, xmm11
	vmovdqa64 ymm8, ymm8
	vmovdqa64 zmm11, zmm15
	vmovdqa64 xmm8, xmm12
	vmovdqa64 xmm31, xmm17
	vmovdqa64 ymm27, ymm24
	vmovdqa64 zmm27, zmm21
	vmovdqa64 xmm31, xmm18
%endif
