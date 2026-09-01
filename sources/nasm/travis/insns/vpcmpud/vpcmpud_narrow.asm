	vpcmpud k6, xmm7, xmm2, 0xda
	vpcmpud k2, xmm3, xmm0, 0xc1
	vpcmpud k4, ymm6, ymm0, 0x33
	vpcmpud k5, ymm3, ymm6, 0x96
	vpcmpud k4, zmm7, zword [0xbd8], 0x4
	vpcmpud k3, zmm4, zmm6, 0x67

%ifdef ERROR
	vpcmpud k3, xmm10, xmm12, 0x6
	vpcmpud k3, ymm13, ymm13, 0xda
	vpcmpud k4, zmm10, zmm12, 0x7d
	vpcmpud k4, xmm20, xmm25, 0x2
	vpcmpud k2, ymm28, ymm20, 0xde
	vpcmpud k6, zmm21, zmm30, 0x99
%endif
