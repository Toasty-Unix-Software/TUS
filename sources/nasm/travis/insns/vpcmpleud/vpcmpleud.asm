default rel
	vpcmpleud k6, xmm6, xmm7
	vpcmpleud k1, xmm5, oword [0xb60]
	vpcmpleud k2, ymm1, ymm0
	vpcmpleud k5, ymm5, ymm3
	vpcmpleud k1, zmm2, zword [0xac1]
	vpcmpleud k2, zmm1, zword [0x8d5]
	vpcmpleud k4, xmm9, xmm13
	vpcmpleud k2, ymm9, ymm9
	vpcmpleud k5, zmm8, zmm11
	vpcmpleud k4, xmm19, xmm30
	vpcmpleud k1, ymm17, ymm16
	vpcmpleud k4, zmm18, zmm24
	vpcmpleud k2{k4}, xmm1, xmm2
	vpcmpleud k3{k2}, ymm3, yword [0xa46]
	vpcmpleud k5{k3}, zmm3, zword [0x61a]
	vpcmpleud k1, xmm5, oword [eax+1]
	vpcmpleud k4, xmm7, oword [eax+64]
	vpcmpleud k7, ymm4, yword [eax+1]
	vpcmpleud k5, ymm7, yword [eax+64]
	vpcmpleud k4, zmm7, zword [eax+1]
	vpcmpleud k2, zmm5, zword [eax+64]
	vpcmpleud k4, xmm4, [0x63d]
	vpcmpleud k7, ymm2, [0x69e]
	vpcmpleud k2, zmm7, [0x499]
