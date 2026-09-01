default rel
	vmovntps oword [0x327], xmm1
	vmovntps oword [0xa5b], xmm5
	vmovntps yword [0x4f2], ymm5
	vmovntps yword [0x4e7], ymm6
	vmovntps zword [0xc9a], zmm2
	vmovntps zword [0x4c4], zmm4
	vmovntps oword [0xa2c], xmm15
	vmovntps yword [0x7da], ymm8
	vmovntps zword [0x672], zmm12
	vmovntps oword [0xd32], xmm22
	vmovntps yword [0x9b4], ymm28
	vmovntps zword [0x826], zmm26
	vmovntps oword [eax+1], xmm1
	vmovntps oword [eax+64], xmm7
	vmovntps yword [eax+1], ymm4
	vmovntps yword [eax+64], ymm4
	vmovntps zword [eax+1], zmm0
	vmovntps zword [eax+64], zmm5
	vmovntps [0x413], xmm1
	vmovntps [0x31a], ymm5
	vmovntps [0x46e], zmm7
