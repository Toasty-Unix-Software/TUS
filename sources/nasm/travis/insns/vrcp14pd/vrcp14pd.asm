default rel
	vrcp14pd xmm2, xmm2
	vrcp14pd xmm2, xmm3
	vrcp14pd ymm1, yword [0xe3c]
	vrcp14pd ymm6, ymm4
	vrcp14pd zmm7, zmm0
	vrcp14pd zmm0, zword [0x2a7]
	vrcp14pd xmm11, xmm9
	vrcp14pd ymm12, ymm15
	vrcp14pd zmm12, zmm8
	vrcp14pd xmm17, xmm20
	vrcp14pd ymm20, ymm30
	vrcp14pd zmm26, zmm23
	vrcp14pd xmm2{k5}, xmm0
	vrcp14pd ymm0{k1}, ymm0
	vrcp14pd zmm7{k7}, zword [0xe30]
	vrcp14pd xmm5{k3}{z}, xmm0
	vrcp14pd ymm0{k3}{z}, yword [0xb04]
	vrcp14pd zmm4{k5}{z}, zmm2
	vrcp14pd xmm7, oword [eax+1]
	vrcp14pd xmm1, oword [eax+64]
	vrcp14pd ymm7, yword [eax+1]
	vrcp14pd ymm3, yword [eax+64]
	vrcp14pd zmm7, zword [eax+1]
	vrcp14pd zmm2, zword [eax+64]
	vrcp14pd xmm6, [0x697]
	vrcp14pd ymm6, [0x7df]
	vrcp14pd zmm7, [0x7fc]
