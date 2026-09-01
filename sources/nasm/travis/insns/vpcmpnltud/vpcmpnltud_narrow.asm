	vpcmpnltud k1, xmm2, xmm0
	vpcmpnltud k2, xmm5, oword [0x445]
	vpcmpnltud k2, ymm1, yword [0xbd9]
	vpcmpnltud k3, ymm0, yword [0x2bb]
	vpcmpnltud k4, zmm6, zmm5
	vpcmpnltud k6, zmm1, zmm2

%ifdef ERROR
	vpcmpnltud k4, xmm15, xmm10
	vpcmpnltud k5, ymm15, ymm8
	vpcmpnltud k1, zmm10, zmm13
	vpcmpnltud k2, xmm19, xmm26
	vpcmpnltud k1, ymm31, ymm30
	vpcmpnltud k2, zmm18, zmm18
%endif
