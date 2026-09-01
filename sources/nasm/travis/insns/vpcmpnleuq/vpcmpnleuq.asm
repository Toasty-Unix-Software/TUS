default rel
	vpcmpnleuq k7, xmm2, oword [0x3d8]
	vpcmpnleuq k2, xmm6, xmm4
	vpcmpnleuq k7, ymm4, yword [0x633]
	vpcmpnleuq k2, ymm5, yword [0xd03]
	vpcmpnleuq k4, zmm4, zword [0x8ad]
	vpcmpnleuq k4, zmm1, zmm3
	vpcmpnleuq k3, xmm14, xmm13
	vpcmpnleuq k7, ymm15, ymm13
	vpcmpnleuq k1, zmm12, zmm8
	vpcmpnleuq k3, xmm19, xmm19
	vpcmpnleuq k7, ymm27, ymm25
	vpcmpnleuq k5, zmm25, zmm25
	vpcmpnleuq k2{k3}, xmm3, xmm3
	vpcmpnleuq k7{k4}, ymm3, ymm0
	vpcmpnleuq k3{k2}, zmm7, zmm5
	vpcmpnleuq k3, xmm4, oword [eax+1]
	vpcmpnleuq k4, xmm4, oword [eax+64]
	vpcmpnleuq k5, ymm0, yword [eax+1]
	vpcmpnleuq k4, ymm2, yword [eax+64]
	vpcmpnleuq k3, zmm5, zword [eax+1]
	vpcmpnleuq k1, zmm4, zword [eax+64]
	vpcmpnleuq k6, xmm2, [0x1e4]
	vpcmpnleuq k6, ymm6, [0x22c]
	vpcmpnleuq k3, zmm0, [0xcc3]
