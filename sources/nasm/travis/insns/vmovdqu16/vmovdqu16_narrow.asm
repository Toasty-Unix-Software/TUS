	vmovdqu16 xmm4, oword [0xa70]
	vmovdqu16 xmm5, xmm6
	vmovdqu16 ymm7, ymm3
	vmovdqu16 ymm4, ymm2
	vmovdqu16 zmm6, zword [0xd0c]
	vmovdqu16 zmm2, zmm3
	vmovdqu16 xmm5, xmm0
	vmovdqu16 oword [0xc8b], xmm4

%ifdef ERROR
	vmovdqu16 xmm8, xmm9
	vmovdqu16 ymm15, ymm14
	vmovdqu16 zmm15, zmm13
	vmovdqu16 xmm14, xmm9
	vmovdqu16 xmm19, xmm18
	vmovdqu16 ymm16, ymm27
	vmovdqu16 zmm25, zmm18
	vmovdqu16 xmm28, xmm29
%endif
