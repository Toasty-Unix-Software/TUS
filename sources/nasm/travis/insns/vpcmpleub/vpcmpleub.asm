default rel
	vpcmpleub k4, xmm7, xmm0
	vpcmpleub k3, xmm1, oword [0xf6e]
	vpcmpleub k6, ymm5, yword [0xebf]
	vpcmpleub k6, ymm0, yword [0xdb7]
	vpcmpleub k1, zmm7, zmm2
	vpcmpleub k2, zmm2, zmm2
	vpcmpleub k3, xmm8, xmm13
	vpcmpleub k1, ymm15, ymm12
	vpcmpleub k3, zmm11, zmm15
	vpcmpleub k2, xmm22, xmm27
	vpcmpleub k5, ymm19, ymm20
	vpcmpleub k3, zmm20, zmm28
	vpcmpleub k4{k1}, xmm4, xmm2
	vpcmpleub k4{k6}, ymm3, ymm3
	vpcmpleub k2{k6}, zmm2, zmm0
	vpcmpleub k1, xmm0, oword [eax+1]
	vpcmpleub k3, xmm5, oword [eax+64]
	vpcmpleub k4, ymm1, yword [eax+1]
	vpcmpleub k4, ymm5, yword [eax+64]
	vpcmpleub k1, zmm6, zword [eax+1]
	vpcmpleub k6, zmm5, zword [eax+64]
	vpcmpleub k7, xmm1, [0xa68]
	vpcmpleub k2, ymm6, [0x38f]
	vpcmpleub k3, zmm3, [0xdf7]
