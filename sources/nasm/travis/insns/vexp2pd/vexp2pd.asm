default rel
	vexp2pd zmm4, zword [0x84f]
	vexp2pd zmm4, zmm5
	vexp2pd zmm15, zmm10
	vexp2pd zmm28, zmm21
	vexp2pd zmm6{k6}, zmm5
	vexp2pd zmm7{k6}{z}, zword [0xf62]
	vexp2pd zmm7, zmm1, {sae}
	vexp2pd zmm6, zword [eax+1]
	vexp2pd zmm5, zword [eax+64]
	vexp2pd zmm2, [0xa37]
