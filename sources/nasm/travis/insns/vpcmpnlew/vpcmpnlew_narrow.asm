	vpcmpnlew k3, xmm0, xmm4
	vpcmpnlew k5, xmm7, xmm4
	vpcmpnlew k7, ymm1, yword [0x925]
	vpcmpnlew k4, ymm5, ymm3
	vpcmpnlew k1, zmm0, zword [0x41f]
	vpcmpnlew k2, zmm2, zmm6

%ifdef ERROR
	vpcmpnlew k2, xmm11, xmm10
	vpcmpnlew k7, ymm12, ymm8
	vpcmpnlew k1, zmm14, zmm14
	vpcmpnlew k5, xmm18, xmm20
	vpcmpnlew k3, ymm16, ymm25
	vpcmpnlew k4, zmm19, zmm31
%endif
