default rel
	vpcmpnlew k3, xmm0, xmm4
	vpcmpnlew k5, xmm7, xmm4
	vpcmpnlew k7, ymm1, yword [0x925]
	vpcmpnlew k4, ymm5, ymm3
	vpcmpnlew k1, zmm0, zword [0x41f]
	vpcmpnlew k2, zmm2, zmm6
	vpcmpnlew k2, xmm11, xmm10
	vpcmpnlew k7, ymm12, ymm8
	vpcmpnlew k1, zmm14, zmm14
	vpcmpnlew k5, xmm18, xmm20
	vpcmpnlew k3, ymm16, ymm25
	vpcmpnlew k4, zmm19, zmm31
	vpcmpnlew k6{k2}, xmm5, oword [0x232]
	vpcmpnlew k5{k5}, ymm0, yword [0x16e]
	vpcmpnlew k3{k1}, zmm3, zword [0x92a]
	vpcmpnlew k2, xmm2, oword [eax+1]
	vpcmpnlew k2, xmm3, oword [eax+64]
	vpcmpnlew k7, ymm3, yword [eax+1]
	vpcmpnlew k6, ymm1, yword [eax+64]
	vpcmpnlew k5, zmm4, zword [eax+1]
	vpcmpnlew k4, zmm0, zword [eax+64]
	vpcmpnlew k3, xmm4, [0xc78]
	vpcmpnlew k7, ymm7, [0x70e]
	vpcmpnlew k4, zmm6, [0xd69]
