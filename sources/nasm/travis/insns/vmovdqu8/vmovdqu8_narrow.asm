	vmovdqu8 xmm5, oword [0x847]
	vmovdqu8 xmm0, xmm5
	vmovdqu8 ymm1, ymm7
	vmovdqu8 ymm0, ymm5
	vmovdqu8 zmm4, zmm4
	vmovdqu8 zmm7, zmm1
	vmovdqu8 xmm0, xmm1
	vmovdqu8 xmm2, xmm6

%ifdef ERROR
	vmovdqu8 xmm9, xmm11
	vmovdqu8 ymm15, ymm14
	vmovdqu8 zmm15, zmm9
	vmovdqu8 xmm15, xmm10
	vmovdqu8 xmm29, xmm16
	vmovdqu8 ymm22, ymm20
	vmovdqu8 zmm21, zmm16
	vmovdqu8 xmm16, xmm26
%endif
