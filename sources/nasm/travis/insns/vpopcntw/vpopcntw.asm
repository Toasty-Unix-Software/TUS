default rel
	vpopcntw xmm7, xmm6
	vpopcntw xmm6, oword [0xc40]
	vpopcntw ymm5, ymm0
	vpopcntw ymm1, ymm0
	vpopcntw zmm7, zmm0
	vpopcntw zmm2, zmm7
	vpopcntw xmm13, xmm14
	vpopcntw ymm9, ymm15
	vpopcntw zmm13, zmm14
	vpopcntw xmm28, xmm29
	vpopcntw ymm24, ymm21
	vpopcntw zmm17, zmm31
	vpopcntw xmm0{k1}, xmm1
	vpopcntw ymm1{k5}, ymm3
	vpopcntw zmm3{k1}, zword [0x8f5]
	vpopcntw xmm0{k2}{z}, oword [0xd33]
	vpopcntw ymm2{k2}{z}, yword [0x137]
	vpopcntw zmm6{k4}{z}, zword [0x71e]
	vpopcntw xmm3, oword [eax+1]
	vpopcntw xmm5, oword [eax+64]
	vpopcntw ymm6, yword [eax+1]
	vpopcntw ymm5, yword [eax+64]
	vpopcntw zmm0, zword [eax+1]
	vpopcntw zmm1, zword [eax+64]
	vpopcntw xmm7, [0xa11]
	vpopcntw ymm1, [0x642]
	vpopcntw zmm4, [0x557]
