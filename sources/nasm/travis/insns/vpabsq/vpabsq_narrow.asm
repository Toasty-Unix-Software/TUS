	vpabsq xmm3, oword [0x567]
	vpabsq xmm6, oword [0xad6]
	vpabsq ymm3, ymm6
	vpabsq ymm6, ymm5
	vpabsq zmm2, zword [0xd27]
	vpabsq zmm3, zmm1

%ifdef ERROR
	vpabsq xmm14, xmm9
	vpabsq ymm14, ymm12
	vpabsq zmm15, zmm12
	vpabsq xmm28, xmm31
	vpabsq ymm26, ymm16
	vpabsq zmm16, zmm20
%endif
