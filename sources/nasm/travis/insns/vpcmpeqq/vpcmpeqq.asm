default rel
	vpcmpeqq xmm2, xmm2, xmm1
	vpcmpeqq xmm2, xmm1
	vpcmpeqq xmm3, xmm3, xmm7
	vpcmpeqq ymm1, ymm4, ymm1
	vpcmpeqq ymm1, ymm1
	vpcmpeqq ymm2, ymm0, ymm6
	vpcmpeqq k1, xmm6, xmm5
	vpcmpeqq k4, xmm2, xmm3
	vpcmpeqq k4, ymm0, ymm5
	vpcmpeqq k1, ymm0, ymm7
	vpcmpeqq xmm12, xmm13, xmm11
	vpcmpeqq ymm8, ymm10, ymm11
	vpcmpeqq k4, xmm8, xmm8
	vpcmpeqq k5, ymm12, ymm9
	vpcmpeqq k2{k2}, xmm4, xmm2
	vpcmpeqq k1{k4}, ymm1, ymm2
	vpcmpeqq k3{k5}, zmm4, zmm4
	vpcmpeqq xmm5, xmm0, oword [eax+1]
	vpcmpeqq xmm5, xmm7, oword [eax+64]
	vpcmpeqq ymm1, ymm2, yword [eax+1]
	vpcmpeqq ymm5, ymm7, yword [eax+64]
	vpcmpeqq k3, xmm4, oword [eax+1]
	vpcmpeqq k1, xmm1, oword [eax+64]
	vpcmpeqq k4, ymm5, yword [eax+1]
	vpcmpeqq k3, ymm5, yword [eax+64]
	vpcmpeqq k2, zmm4, zword [eax+1]
	vpcmpeqq k2, zmm4, zword [eax+64]
	vpcmpeqq xmm0, xmm1, [0xd79]
	vpcmpeqq ymm5, ymm6, [0x954]
	vpcmpeqq k1, xmm6, [0x6a7]
	vpcmpeqq k4, ymm0, [0x3cb]
	vpcmpeqq k2, zmm0, [0xfc3]
