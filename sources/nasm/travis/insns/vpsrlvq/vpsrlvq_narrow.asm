	vpsrlvq xmm4, xmm2, xmm2
	vpsrlvq xmm4, xmm2
	vpsrlvq xmm5, xmm3, xmm1
	vpsrlvq ymm1, ymm4, ymm6
	vpsrlvq ymm1, ymm6
	vpsrlvq ymm4, ymm0, ymm2
	vpsrlvq xmm4, xmm4, xmm0
	vpsrlvq xmm4, xmm0
	vpsrlvq xmm4, xmm4, oword [0x9b8]
	vpsrlvq ymm0, ymm3, ymm1
	vpsrlvq ymm0, ymm1
	vpsrlvq ymm0, ymm5, ymm2

%ifdef ERROR
	vpsrlvq xmm10, xmm9, xmm14
	vpsrlvq ymm11, ymm10, ymm9
	vpsrlvq xmm14, xmm15, xmm15
	vpsrlvq ymm10, ymm15, ymm10
	vpsrlvq xmm18, xmm27, xmm18
	vpsrlvq ymm29, ymm17, ymm26
	vpsrlvq xmm18, xmm16, xmm20
	vpsrlvq ymm29, ymm31, ymm21
%endif
