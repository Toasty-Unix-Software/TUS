default rel
	vpermt2pd xmm0, xmm2, oword [0xc75]
	vpermt2pd xmm2, xmm0, xmm0
	vpermt2pd ymm3, ymm5, ymm2
	vpermt2pd ymm5, ymm5, ymm1
	vpermt2pd zmm7, zmm0, zmm1
	vpermt2pd zmm6, zmm4, zmm3
	vpermt2pd xmm15, xmm10, xmm10
	vpermt2pd ymm11, ymm13, ymm13
	vpermt2pd zmm8, zmm15, zmm12
	vpermt2pd xmm28, xmm18, xmm27
	vpermt2pd ymm16, ymm30, ymm26
	vpermt2pd zmm23, zmm19, zmm16
	vpermt2pd xmm5{k3}, xmm1, xmm5
	vpermt2pd ymm5{k4}, ymm5, ymm0
	vpermt2pd zmm6{k5}, zmm0, zmm4
	vpermt2pd xmm4{k7}{z}, xmm5, xmm5
	vpermt2pd ymm6{k1}{z}, ymm7, ymm1
	vpermt2pd zmm4{k3}{z}, zmm7, zmm1
	vpermt2pd xmm2, xmm0, oword [eax+1]
	vpermt2pd xmm5, xmm6, oword [eax+64]
	vpermt2pd ymm2, ymm7, yword [eax+1]
	vpermt2pd ymm7, ymm7, yword [eax+64]
	vpermt2pd zmm1, zmm0, zword [eax+1]
	vpermt2pd zmm6, zmm3, zword [eax+64]
	vpermt2pd xmm0, xmm0, [0xefe]
	vpermt2pd ymm7, ymm1, [0x5d4]
	vpermt2pd zmm5, zmm6, [0xff9]
