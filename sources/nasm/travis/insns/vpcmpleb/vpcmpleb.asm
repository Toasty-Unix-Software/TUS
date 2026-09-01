default rel
	vpcmpleb k5, xmm2, xmm0
	vpcmpleb k4, xmm3, xmm5
	vpcmpleb k3, ymm4, ymm0
	vpcmpleb k6, ymm4, ymm5
	vpcmpleb k5, zmm2, zmm6
	vpcmpleb k5, zmm2, zmm1
	vpcmpleb k1, xmm15, xmm9
	vpcmpleb k4, ymm15, ymm11
	vpcmpleb k1, zmm10, zmm8
	vpcmpleb k7, xmm19, xmm20
	vpcmpleb k7, ymm16, ymm24
	vpcmpleb k5, zmm21, zmm19
	vpcmpleb k2{k2}, xmm0, oword [0x5d1]
	vpcmpleb k3{k3}, ymm1, ymm1
	vpcmpleb k2{k7}, zmm7, zmm4
	vpcmpleb k7, xmm2, oword [eax+1]
	vpcmpleb k1, xmm6, oword [eax+64]
	vpcmpleb k5, ymm3, yword [eax+1]
	vpcmpleb k6, ymm5, yword [eax+64]
	vpcmpleb k4, zmm7, zword [eax+1]
	vpcmpleb k5, zmm4, zword [eax+64]
	vpcmpleb k2, xmm0, [0x95c]
	vpcmpleb k6, ymm5, [0x109]
	vpcmpleb k1, zmm3, [0x2cf]
