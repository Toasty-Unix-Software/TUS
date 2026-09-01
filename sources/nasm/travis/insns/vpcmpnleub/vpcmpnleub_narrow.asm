	vpcmpnleub k5, xmm3, xmm0
	vpcmpnleub k3, xmm3, xmm3
	vpcmpnleub k4, ymm3, yword [0xd59]
	vpcmpnleub k4, ymm6, ymm5
	vpcmpnleub k1, zmm5, zmm4
	vpcmpnleub k5, zmm4, zword [0xf06]

%ifdef ERROR
	vpcmpnleub k5, xmm13, xmm10
	vpcmpnleub k6, ymm9, ymm8
	vpcmpnleub k7, zmm11, zmm12
	vpcmpnleub k5, xmm27, xmm18
	vpcmpnleub k2, ymm26, ymm24
	vpcmpnleub k2, zmm28, zmm19
%endif
