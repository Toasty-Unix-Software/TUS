default rel
	vexp2ps zmm1, zword [0xd3e]
	vexp2ps zmm5, zmm1
	vexp2ps zmm11, zmm12
	vexp2ps zmm29, zmm25
	vexp2ps zmm0{k6}, zword [0xa84]
	vexp2ps zmm7{k5}{z}, zword [0xace]
	vexp2ps zmm6, zmm5, {sae}
	vexp2ps zmm1, zword [eax+1]
	vexp2ps zmm4, zword [eax+64]
	vexp2ps zmm0, [0x61f]
