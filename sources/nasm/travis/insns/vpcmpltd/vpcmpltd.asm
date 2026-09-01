default rel
	vpcmpltd k7, xmm3, xmm1
	vpcmpltd k4, xmm1, xmm4
	vpcmpltd k3, ymm3, ymm3
	vpcmpltd k7, ymm0, ymm0
	vpcmpltd k1, zmm1, zword [0x274]
	vpcmpltd k7, zmm0, zmm2
	vpcmpltd k2, xmm11, xmm14
	vpcmpltd k5, ymm14, ymm15
	vpcmpltd k2, zmm8, zmm14
	vpcmpltd k3, xmm20, xmm20
	vpcmpltd k4, ymm16, ymm21
	vpcmpltd k2, zmm28, zmm16
	vpcmpltd k5{k6}, xmm2, xmm3
	vpcmpltd k4{k5}, ymm2, ymm2
	vpcmpltd k3{k2}, zmm7, zmm5
	vpcmpltd k5, xmm3, oword [eax+1]
	vpcmpltd k4, xmm3, oword [eax+64]
	vpcmpltd k4, ymm6, yword [eax+1]
	vpcmpltd k5, ymm5, yword [eax+64]
	vpcmpltd k3, zmm0, zword [eax+1]
	vpcmpltd k5, zmm6, zword [eax+64]
	vpcmpltd k7, xmm7, [0x366]
	vpcmpltd k4, ymm3, [0x5dc]
	vpcmpltd k2, zmm6, [0xf1c]
