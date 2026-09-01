default rel
	vpcmpeqw xmm0, xmm3, xmm2
	vpcmpeqw xmm0, xmm2
	vpcmpeqw xmm2, xmm6, xmm2
	vpcmpeqw ymm7, ymm4, ymm6
	vpcmpeqw ymm7, ymm6
	vpcmpeqw ymm6, ymm4, ymm0
	vpcmpeqw k3, xmm2, oword [0x83b]
	vpcmpeqw k6, xmm1, oword [0x434]
	vpcmpeqw k4, ymm7, yword [0x76b]
	vpcmpeqw k3, ymm4, ymm2
	vpcmpeqw xmm15, xmm15, xmm8
	vpcmpeqw ymm13, ymm14, ymm14
	vpcmpeqw k4, xmm15, xmm13
	vpcmpeqw k5, ymm8, ymm11
	vpcmpeqw k1{k1}, xmm2, oword [0xfdd]
	vpcmpeqw k5{k6}, ymm6, yword [0x7de]
	vpcmpeqw k3{k3}, zmm7, zmm5
	vpcmpeqw xmm7, xmm0, oword [eax+1]
	vpcmpeqw xmm6, xmm3, oword [eax+64]
	vpcmpeqw ymm0, ymm2, yword [eax+1]
	vpcmpeqw ymm3, ymm7, yword [eax+64]
	vpcmpeqw k4, xmm5, oword [eax+1]
	vpcmpeqw k1, xmm0, oword [eax+64]
	vpcmpeqw k2, ymm4, yword [eax+1]
	vpcmpeqw k5, ymm3, yword [eax+64]
	vpcmpeqw k7, zmm0, zword [eax+1]
	vpcmpeqw k6, zmm5, zword [eax+64]
	vpcmpeqw xmm7, xmm7, [0x31c]
	vpcmpeqw ymm2, ymm3, [0x382]
	vpcmpeqw k3, xmm3, [0xe38]
	vpcmpeqw k5, ymm1, [0x141]
	vpcmpeqw k7, zmm2, [0x782]
