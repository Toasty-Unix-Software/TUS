default rel
	vrsqrt28pd zmm2, zword [0xd93]
	vrsqrt28pd zmm4, zword [0x470]
	vrsqrt28pd zmm12, zmm11
	vrsqrt28pd zmm29, zmm22
	vrsqrt28pd zmm5{k7}, zmm3
	vrsqrt28pd zmm6{k1}{z}, zmm0
	vrsqrt28pd zmm1, zmm6, {sae}
	vrsqrt28pd zmm1, zword [eax+1]
	vrsqrt28pd zmm2, zword [eax+64]
	vrsqrt28pd zmm5, [0x1d5]
