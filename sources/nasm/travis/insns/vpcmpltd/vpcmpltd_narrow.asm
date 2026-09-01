	vpcmpltd k7, xmm3, xmm1
	vpcmpltd k4, xmm1, xmm4
	vpcmpltd k3, ymm3, ymm3
	vpcmpltd k7, ymm0, ymm0
	vpcmpltd k1, zmm1, zword [0x274]
	vpcmpltd k7, zmm0, zmm2

%ifdef ERROR
	vpcmpltd k2, xmm11, xmm14
	vpcmpltd k5, ymm14, ymm15
	vpcmpltd k2, zmm8, zmm14
	vpcmpltd k3, xmm20, xmm20
	vpcmpltd k4, ymm16, ymm21
	vpcmpltd k2, zmm28, zmm16
%endif
