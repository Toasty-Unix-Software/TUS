	vpcmpnltuw k3, xmm4, oword [0xe37]
	vpcmpnltuw k1, xmm7, xmm6
	vpcmpnltuw k4, ymm3, ymm3
	vpcmpnltuw k3, ymm3, ymm6
	vpcmpnltuw k1, zmm5, zword [0x45a]
	vpcmpnltuw k6, zmm5, zmm2

%ifdef ERROR
	vpcmpnltuw k7, xmm8, xmm13
	vpcmpnltuw k6, ymm10, ymm15
	vpcmpnltuw k5, zmm14, zmm13
	vpcmpnltuw k1, xmm21, xmm30
	vpcmpnltuw k1, ymm16, ymm16
	vpcmpnltuw k7, zmm28, zmm20
%endif
