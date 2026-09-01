	vmovupd xmm0, xmm0
	vmovupd xmm6, oword [0x19a]
	vmovupd xmm3, xmm4
	vmovupd xmm6, xmm1
	vmovupd ymm0, ymm5
	vmovupd ymm2, ymm4
	vmovupd ymm2, ymm3
	vmovupd ymm2, ymm3

%ifdef ERROR
	vmovupd xmm8, xmm13
	vmovupd xmm9, xmm14
	vmovupd ymm9, ymm15
	vmovupd ymm12, ymm8
	vmovupd xmm30, xmm22
	vmovupd xmm21, xmm23
	vmovupd ymm17, ymm22
	vmovupd ymm20, ymm26
%endif
