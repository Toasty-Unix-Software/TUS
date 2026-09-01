default rel
	vrsqrt14pd xmm2, xmm6
	vrsqrt14pd xmm2, xmm4
	vrsqrt14pd ymm0, ymm4
	vrsqrt14pd ymm7, ymm3
	vrsqrt14pd zmm1, zmm3
	vrsqrt14pd zmm5, zmm1
	vrsqrt14pd xmm15, xmm11
	vrsqrt14pd ymm11, ymm8
	vrsqrt14pd zmm14, zmm11
	vrsqrt14pd xmm29, xmm17
	vrsqrt14pd ymm23, ymm21
	vrsqrt14pd zmm19, zmm20
	vrsqrt14pd xmm6{k5}, xmm4
	vrsqrt14pd ymm5{k4}, yword [0x7fc]
	vrsqrt14pd zmm1{k7}, zmm0
	vrsqrt14pd xmm6{k4}{z}, xmm2
	vrsqrt14pd ymm2{k2}{z}, ymm7
	vrsqrt14pd zmm3{k1}{z}, zmm5
	vrsqrt14pd xmm2, oword [eax+1]
	vrsqrt14pd xmm6, oword [eax+64]
	vrsqrt14pd ymm3, yword [eax+1]
	vrsqrt14pd ymm7, yword [eax+64]
	vrsqrt14pd zmm0, zword [eax+1]
	vrsqrt14pd zmm0, zword [eax+64]
	vrsqrt14pd xmm7, [0x20f]
	vrsqrt14pd ymm1, [0x353]
	vrsqrt14pd zmm4, [0xba9]
