	vpcmpnleq k4, xmm4, oword [0xddf]
	vpcmpnleq k4, xmm3, oword [0xb94]
	vpcmpnleq k1, ymm4, ymm0
	vpcmpnleq k3, ymm5, ymm6
	vpcmpnleq k2, zmm6, zword [0x525]
	vpcmpnleq k4, zmm2, zword [0x16c]

%ifdef ERROR
	vpcmpnleq k6, xmm12, xmm10
	vpcmpnleq k2, ymm9, ymm15
	vpcmpnleq k7, zmm14, zmm15
	vpcmpnleq k1, xmm27, xmm30
	vpcmpnleq k6, ymm18, ymm27
	vpcmpnleq k3, zmm19, zmm16
%endif
