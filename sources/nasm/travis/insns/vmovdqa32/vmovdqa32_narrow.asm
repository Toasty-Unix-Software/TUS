	vmovdqa32 xmm6, xmm6
	vmovdqa32 xmm7, xmm6
	vmovdqa32 ymm0, ymm4
	vmovdqa32 ymm4, yword [0xb90]
	vmovdqa32 zmm0, zmm6
	vmovdqa32 zmm1, zmm0
	vmovdqa32 oword [0xc5e], xmm6
	vmovdqa32 xmm2, xmm3

%ifdef ERROR
	vmovdqa32 xmm13, xmm10
	vmovdqa32 ymm10, ymm10
	vmovdqa32 zmm9, zmm15
	vmovdqa32 xmm9, xmm14
	vmovdqa32 xmm20, xmm25
	vmovdqa32 ymm21, ymm17
	vmovdqa32 zmm22, zmm18
	vmovdqa32 xmm16, xmm26
%endif
