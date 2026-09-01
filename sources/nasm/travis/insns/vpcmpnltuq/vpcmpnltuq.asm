default rel
	vpcmpnltuq k4, xmm2, xmm6
	vpcmpnltuq k6, xmm4, xmm6
	vpcmpnltuq k2, ymm2, ymm6
	vpcmpnltuq k1, ymm1, ymm7
	vpcmpnltuq k6, zmm2, zmm6
	vpcmpnltuq k3, zmm4, zword [0xbcc]
	vpcmpnltuq k7, xmm14, xmm13
	vpcmpnltuq k4, ymm11, ymm13
	vpcmpnltuq k5, zmm15, zmm14
	vpcmpnltuq k5, xmm23, xmm30
	vpcmpnltuq k4, ymm25, ymm31
	vpcmpnltuq k5, zmm20, zmm29
	vpcmpnltuq k1{k2}, xmm6, xmm3
	vpcmpnltuq k1{k2}, ymm1, ymm1
	vpcmpnltuq k2{k6}, zmm6, zmm4
	vpcmpnltuq k2, xmm6, oword [eax+1]
	vpcmpnltuq k5, xmm2, oword [eax+64]
	vpcmpnltuq k4, ymm7, yword [eax+1]
	vpcmpnltuq k3, ymm6, yword [eax+64]
	vpcmpnltuq k3, zmm4, zword [eax+1]
	vpcmpnltuq k2, zmm7, zword [eax+64]
	vpcmpnltuq k6, xmm3, [0x8d3]
	vpcmpnltuq k2, ymm6, [0xf48]
	vpcmpnltuq k5, zmm7, [0xd30]
