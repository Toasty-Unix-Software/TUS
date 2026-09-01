default rel
	vpcmpnleud k2, xmm5, xmm3
	vpcmpnleud k4, xmm4, oword [0x276]
	vpcmpnleud k5, ymm3, ymm1
	vpcmpnleud k2, ymm6, ymm1
	vpcmpnleud k1, zmm5, zword [0x18b]
	vpcmpnleud k7, zmm7, zmm0
	vpcmpnleud k1, xmm14, xmm10
	vpcmpnleud k4, ymm13, ymm14
	vpcmpnleud k4, zmm8, zmm11
	vpcmpnleud k1, xmm20, xmm28
	vpcmpnleud k4, ymm21, ymm19
	vpcmpnleud k1, zmm27, zmm26
	vpcmpnleud k2{k6}, xmm2, oword [0x3ca]
	vpcmpnleud k6{k7}, ymm0, ymm0
	vpcmpnleud k5{k2}, zmm5, zword [0x892]
	vpcmpnleud k1, xmm6, oword [eax+1]
	vpcmpnleud k2, xmm2, oword [eax+64]
	vpcmpnleud k5, ymm3, yword [eax+1]
	vpcmpnleud k5, ymm4, yword [eax+64]
	vpcmpnleud k5, zmm4, zword [eax+1]
	vpcmpnleud k4, zmm3, zword [eax+64]
	vpcmpnleud k4, xmm7, [0x744]
	vpcmpnleud k6, ymm5, [0x9d0]
	vpcmpnleud k3, zmm0, [0xca3]
