	vpcmpuq k3, xmm5, xmm5, 0xb9
	vpcmpuq k2, xmm3, xmm5, 0xfd
	vpcmpuq k5, ymm3, yword [0x380], 0x75
	vpcmpuq k6, ymm0, ymm1, 0x2e
	vpcmpuq k1, zmm6, zword [0x53c], 0x64
	vpcmpuq k6, zmm5, zmm1, 0x48

%ifdef ERROR
	vpcmpuq k1, xmm9, xmm13, 0x89
	vpcmpuq k2, ymm9, ymm10, 0xba
	vpcmpuq k1, zmm14, zmm10, 0xc6
	vpcmpuq k3, xmm21, xmm27, 0x6
	vpcmpuq k2, ymm29, ymm19, 0x38
	vpcmpuq k1, zmm25, zmm25, 0xba
%endif
