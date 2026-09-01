default rel
	vpcmpeqd xmm2, xmm6, oword [0xcc5]
	vpcmpeqd xmm2, oword [0xcc5]
	vpcmpeqd xmm2, xmm5, xmm1
	vpcmpeqd ymm2, ymm5, ymm0
	vpcmpeqd ymm2, ymm0
	vpcmpeqd ymm2, ymm6, yword [0x70d]
	vpcmpeqd k3, xmm1, xmm5
	vpcmpeqd k2, xmm1, xmm2
	vpcmpeqd k6, ymm5, ymm0
	vpcmpeqd k3, ymm7, ymm0
	vpcmpeqd xmm13, xmm11, xmm15
	vpcmpeqd ymm13, ymm8, ymm14
	vpcmpeqd k5, xmm8, xmm10
	vpcmpeqd k2, ymm8, ymm15
	vpcmpeqd k4{k4}, xmm5, xmm5
	vpcmpeqd k4{k3}, ymm1, yword [0x23b]
	vpcmpeqd k5{k7}, zmm4, zword [0x843]
	vpcmpeqd xmm2, xmm5, oword [eax+1]
	vpcmpeqd xmm0, xmm1, oword [eax+64]
	vpcmpeqd ymm4, ymm1, yword [eax+1]
	vpcmpeqd ymm4, ymm7, yword [eax+64]
	vpcmpeqd k7, xmm1, oword [eax+1]
	vpcmpeqd k5, xmm0, oword [eax+64]
	vpcmpeqd k4, ymm5, yword [eax+1]
	vpcmpeqd k5, ymm3, yword [eax+64]
	vpcmpeqd k2, zmm2, zword [eax+1]
	vpcmpeqd k1, zmm3, zword [eax+64]
	vpcmpeqd xmm4, xmm5, [0x1e3]
	vpcmpeqd ymm7, ymm5, [0x4af]
	vpcmpeqd k2, xmm1, [0x9de]
	vpcmpeqd k1, ymm4, [0x26e]
	vpcmpeqd k1, zmm7, [0x37f]
