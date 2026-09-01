	vpcmpneqb k4, xmm5, xmm6
	vpcmpneqb k5, xmm6, xmm5
	vpcmpneqb k4, ymm2, yword [0x81a]
	vpcmpneqb k2, ymm1, yword [0x809]
	vpcmpneqb k3, zmm5, zmm0
	vpcmpneqb k1, zmm3, zword [0x95c]

%ifdef ERROR
	vpcmpneqb k6, xmm9, xmm8
	vpcmpneqb k3, ymm14, ymm13
	vpcmpneqb k4, zmm10, zmm12
	vpcmpneqb k2, xmm23, xmm24
	vpcmpneqb k2, ymm28, ymm19
	vpcmpneqb k3, zmm31, zmm31
%endif
