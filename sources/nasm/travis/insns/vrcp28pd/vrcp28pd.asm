default rel
	vrcp28pd zmm2, zmm0
	vrcp28pd zmm6, zword [0x154]
	vrcp28pd zmm8, zmm10
	vrcp28pd zmm20, zmm27
	vrcp28pd zmm5{k4}, zmm6
	vrcp28pd zmm7{k3}{z}, zword [0x693]
	vrcp28pd zmm6, zmm5, {sae}
	vrcp28pd zmm4, zword [eax+1]
	vrcp28pd zmm6, zword [eax+64]
	vrcp28pd zmm3, [0xe95]
