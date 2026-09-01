	vmovntps oword [0x327], xmm1
	vmovntps oword [0xa5b], xmm5
	vmovntps yword [0x4f2], ymm5
	vmovntps yword [0x4e7], ymm6
	vmovntps zword [0xc9a], zmm2
	vmovntps zword [0x4c4], zmm4

%ifdef ERROR
	vmovntps oword [0xa2c], xmm15
	vmovntps yword [0x7da], ymm8
	vmovntps zword [0x672], zmm12
	vmovntps oword [0xd32], xmm22
	vmovntps yword [0x9b4], ymm28
	vmovntps zword [0x826], zmm26
%endif
