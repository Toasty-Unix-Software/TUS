default rel
	vpcmpgtw xmm3, xmm7, oword [0x658]
	vpcmpgtw xmm3, oword [0x658]
	vpcmpgtw xmm7, xmm0, oword [0x9d6]
	vpcmpgtw ymm2, ymm0, ymm5
	vpcmpgtw ymm2, ymm5
	vpcmpgtw ymm3, ymm2, ymm5
	vpcmpgtw k4, xmm5, oword [0xb40]
	vpcmpgtw k4, xmm2, xmm2
	vpcmpgtw k2, ymm5, yword [0x2c0]
	vpcmpgtw k3, ymm3, ymm1
	vpcmpgtw xmm14, xmm9, xmm10
	vpcmpgtw ymm15, ymm11, ymm10
	vpcmpgtw k4, xmm8, xmm15
	vpcmpgtw k4, ymm9, ymm12
	vpcmpgtw k7{k7}, xmm4, xmm1
	vpcmpgtw k5{k2}, ymm7, yword [0x6a1]
	vpcmpgtw k5{k7}, zmm0, zmm1
	vpcmpgtw xmm1, xmm2, oword [eax+1]
	vpcmpgtw xmm7, xmm3, oword [eax+64]
	vpcmpgtw ymm2, ymm0, yword [eax+1]
	vpcmpgtw ymm4, ymm3, yword [eax+64]
	vpcmpgtw k6, xmm6, oword [eax+1]
	vpcmpgtw k1, xmm3, oword [eax+64]
	vpcmpgtw k7, ymm6, yword [eax+1]
	vpcmpgtw k4, ymm5, yword [eax+64]
	vpcmpgtw k7, zmm7, zword [eax+1]
	vpcmpgtw k7, zmm7, zword [eax+64]
	vpcmpgtw xmm6, xmm3, [0x557]
	vpcmpgtw ymm4, ymm5, [0xb0f]
	vpcmpgtw k7, xmm5, [0xbbe]
	vpcmpgtw k7, ymm6, [0xc6a]
	vpcmpgtw k1, zmm4, [0xb5e]
