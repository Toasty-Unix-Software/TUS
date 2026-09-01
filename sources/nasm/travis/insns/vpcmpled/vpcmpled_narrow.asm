	vpcmpled k2, xmm1, xmm6
	vpcmpled k1, xmm7, oword [0x664]
	vpcmpled k7, ymm3, yword [0x62a]
	vpcmpled k7, ymm1, ymm3
	vpcmpled k3, zmm0, zword [0x9a2]
	vpcmpled k4, zmm5, zword [0xfa1]

%ifdef ERROR
	vpcmpled k3, xmm9, xmm12
	vpcmpled k3, ymm10, ymm15
	vpcmpled k6, zmm15, zmm12
	vpcmpled k3, xmm23, xmm31
	vpcmpled k5, ymm20, ymm21
	vpcmpled k2, zmm22, zmm17
%endif
