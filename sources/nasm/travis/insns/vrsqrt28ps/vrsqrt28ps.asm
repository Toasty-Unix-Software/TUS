default rel
	vrsqrt28ps zmm4, zmm1
	vrsqrt28ps zmm3, zword [0xec9]
	vrsqrt28ps zmm15, zmm8
	vrsqrt28ps zmm16, zmm27
	vrsqrt28ps zmm1{k1}, zmm5
	vrsqrt28ps zmm7{k6}{z}, zword [0x8de]
	vrsqrt28ps zmm0, zmm5, {sae}
	vrsqrt28ps zmm6, zword [eax+1]
	vrsqrt28ps zmm0, zword [eax+64]
	vrsqrt28ps zmm3, [0x490]
