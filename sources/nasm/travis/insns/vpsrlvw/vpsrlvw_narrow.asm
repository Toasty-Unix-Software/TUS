	vpsrlvw xmm1, xmm6, xmm2
	vpsrlvw xmm1, xmm2
	vpsrlvw xmm0, xmm5, xmm1
	vpsrlvw ymm2, ymm0, ymm2
	vpsrlvw ymm2, ymm2
	vpsrlvw ymm7, ymm1, ymm6
	vpsrlvw zmm3, zmm5, zmm2
	vpsrlvw zmm3, zmm2
	vpsrlvw zmm1, zmm5, zword [0xe81]

%ifdef ERROR
	vpsrlvw xmm12, xmm8, xmm11
	vpsrlvw ymm14, ymm11, ymm14
	vpsrlvw zmm8, zmm10, zmm10
	vpsrlvw xmm30, xmm17, xmm29
	vpsrlvw ymm31, ymm28, ymm17
	vpsrlvw zmm23, zmm30, zmm20
%endif
