	vpcmpnltub k7, xmm6, oword [0x9e9]
	vpcmpnltub k7, xmm0, xmm5
	vpcmpnltub k3, ymm5, ymm7
	vpcmpnltub k4, ymm6, yword [0xdca]
	vpcmpnltub k5, zmm4, zword [0xdbf]
	vpcmpnltub k3, zmm1, zmm6

%ifdef ERROR
	vpcmpnltub k2, xmm14, xmm9
	vpcmpnltub k3, ymm12, ymm13
	vpcmpnltub k1, zmm8, zmm12
	vpcmpnltub k4, xmm26, xmm19
	vpcmpnltub k6, ymm26, ymm25
	vpcmpnltub k6, zmm24, zmm27
%endif
