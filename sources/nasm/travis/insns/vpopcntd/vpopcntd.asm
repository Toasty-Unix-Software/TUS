default rel
	vpopcntd xmm0, xmm2
	vpopcntd xmm0, oword [0x7fd]
	vpopcntd ymm4, ymm7
	vpopcntd ymm6, ymm3
	vpopcntd zmm5, zword [0x4c1]
	vpopcntd zmm4, zmm0
	vpopcntd xmm10, xmm15
	vpopcntd ymm10, ymm8
	vpopcntd zmm9, zmm10
	vpopcntd xmm30, xmm23
	vpopcntd ymm29, ymm26
	vpopcntd zmm20, zmm20
	vpopcntd xmm4{k3}, xmm1
	vpopcntd ymm0{k3}, ymm2
	vpopcntd zmm7{k7}, zmm7
	vpopcntd xmm4{k7}{z}, xmm1
	vpopcntd ymm2{k6}{z}, ymm0
	vpopcntd zmm4{k1}{z}, zmm2
	vpopcntd xmm5, oword [eax+1]
	vpopcntd xmm1, oword [eax+64]
	vpopcntd ymm6, yword [eax+1]
	vpopcntd ymm7, yword [eax+64]
	vpopcntd zmm7, zword [eax+1]
	vpopcntd zmm6, zword [eax+64]
	vpopcntd xmm0, [0x696]
	vpopcntd ymm1, [0x510]
	vpopcntd zmm3, [0x852]
