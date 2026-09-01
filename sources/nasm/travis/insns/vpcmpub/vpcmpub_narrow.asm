	vpcmpub k3, xmm5, xmm6, 0x28
	vpcmpub k6, xmm0, oword [0xaab], 0xd0
	vpcmpub k4, ymm4, ymm1, 0x5b
	vpcmpub k3, ymm4, ymm6, 0xc0
	vpcmpub k3, zmm2, zmm1, 0x40
	vpcmpub k4, zmm2, zmm0, 0x75

%ifdef ERROR
	vpcmpub k3, xmm8, xmm14, 0x2a
	vpcmpub k5, ymm12, ymm15, 0x41
	vpcmpub k3, zmm9, zmm13, 0xb3
	vpcmpub k3, xmm31, xmm18, 0xd2
	vpcmpub k5, ymm22, ymm23, 0x57
	vpcmpub k6, zmm19, zmm25, 0xe
%endif
