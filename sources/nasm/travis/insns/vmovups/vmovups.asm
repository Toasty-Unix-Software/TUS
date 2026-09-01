default rel
	vmovups xmm4, xmm7
	vmovups xmm2, xmm4
	vmovups xmm2, xmm3
	vmovups xmm2, xmm1
	vmovups ymm1, yword [0x69d]
	vmovups ymm4, yword [0xa8e]
	vmovups ymm2, ymm0
	vmovups yword [0x93b], ymm6
	vmovups xmm10, xmm13
	vmovups xmm10, xmm8
	vmovups ymm11, ymm11
	vmovups ymm9, ymm14
	vmovups xmm29, xmm20
	vmovups xmm24, xmm20
	vmovups ymm17, ymm18
	vmovups ymm17, ymm29
	vmovups xmm3{k6}, xmm7
	vmovups ymm6{k3}, ymm7
	vmovups zmm6{k5}, zmm2
	vmovups xmm5{k6}, xmm6
	vmovups ymm0{k3}, ymm5
	vmovups zmm2{k2}, zmm3
	vmovups oword [0x5c0]{k5}, xmm0
	vmovups yword [0x6b9]{k4}, ymm3
	vmovups zword [0x14d]{k5}, zmm2
	vmovups xmm7, oword [eax+1]
	vmovups xmm1, oword [eax+64]
	vmovups oword [eax+1], xmm4
	vmovups oword [eax+64], xmm4
	vmovups ymm0, yword [eax+1]
	vmovups ymm7, yword [eax+64]
	vmovups yword [eax+1], ymm7
	vmovups yword [eax+64], ymm5
	vmovups xmm7, oword [eax+1]
	vmovups xmm0, oword [eax+64]
	vmovups ymm0, yword [eax+1]
	vmovups ymm7, yword [eax+64]
	vmovups zmm2, zword [eax+1]
	vmovups zmm5, zword [eax+64]
	vmovups oword [eax+1], xmm4
	vmovups oword [eax+64], xmm5
	vmovups yword [eax+1], ymm3
	vmovups yword [eax+64], ymm4
	vmovups zword [eax+1], zmm1
	vmovups zword [eax+64], zmm0
	vmovups xmm0, [0x5d6]
	vmovups [0x1e2], xmm2
	vmovups ymm4, [0x28d]
	vmovups [0x17a], ymm7
	vmovups xmm7, [0xa39]
	vmovups ymm0, [0xea9]
	vmovups zmm7, [0xd83]
	vmovups [0x515], xmm6
	vmovups [0xb82], ymm4
	vmovups [0x4a3], zmm4
