	vpcmpnleuq k7, xmm2, oword [0x3d8]
	vpcmpnleuq k2, xmm6, xmm4
	vpcmpnleuq k7, ymm4, yword [0x633]
	vpcmpnleuq k2, ymm5, yword [0xd03]
	vpcmpnleuq k4, zmm4, zword [0x8ad]
	vpcmpnleuq k4, zmm1, zmm3

%ifdef ERROR
	vpcmpnleuq k3, xmm14, xmm13
	vpcmpnleuq k7, ymm15, ymm13
	vpcmpnleuq k1, zmm12, zmm8
	vpcmpnleuq k3, xmm19, xmm19
	vpcmpnleuq k7, ymm27, ymm25
	vpcmpnleuq k5, zmm25, zmm25
%endif
