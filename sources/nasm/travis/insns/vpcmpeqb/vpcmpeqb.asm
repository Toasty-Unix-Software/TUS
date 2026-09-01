default rel
	vpcmpeqb xmm4, xmm5, xmm0
	vpcmpeqb xmm4, xmm0
	vpcmpeqb xmm2, xmm0, xmm6
	vpcmpeqb ymm7, ymm0, yword [0x4f8]
	vpcmpeqb ymm7, yword [0x4f8]
	vpcmpeqb ymm5, ymm3, ymm3
	vpcmpeqb k3, xmm0, xmm7
	vpcmpeqb k2, xmm0, oword [0xa2f]
	vpcmpeqb k1, ymm4, ymm5
	vpcmpeqb k6, ymm0, yword [0x87a]
	vpcmpeqb xmm10, xmm14, xmm14
	vpcmpeqb ymm11, ymm12, ymm15
	vpcmpeqb k5, xmm12, xmm8
	vpcmpeqb k1, ymm14, ymm9
	vpcmpeqb k4{k1}, xmm5, xmm4
	vpcmpeqb k2{k6}, ymm0, yword [0x791]
	vpcmpeqb k7{k7}, zmm2, zword [0xd9c]
	vpcmpeqb xmm0, xmm6, oword [eax+1]
	vpcmpeqb xmm7, xmm5, oword [eax+64]
	vpcmpeqb ymm5, ymm2, yword [eax+1]
	vpcmpeqb ymm6, ymm5, yword [eax+64]
	vpcmpeqb k5, xmm0, oword [eax+1]
	vpcmpeqb k6, xmm6, oword [eax+64]
	vpcmpeqb k2, ymm1, yword [eax+1]
	vpcmpeqb k3, ymm7, yword [eax+64]
	vpcmpeqb k7, zmm4, zword [eax+1]
	vpcmpeqb k2, zmm5, zword [eax+64]
	vpcmpeqb xmm2, xmm5, [0x85d]
	vpcmpeqb ymm7, ymm2, [0xaff]
	vpcmpeqb k7, xmm0, [0xd86]
	vpcmpeqb k7, ymm0, [0xfcc]
	vpcmpeqb k7, zmm6, [0x4b5]
