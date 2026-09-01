	vptestmb k6, xmm1, oword [0xb8b]
	vptestmb k5, xmm5, xmm5
	vptestmb k4, ymm5, ymm3
	vptestmb k4, ymm1, ymm4
	vptestmb k4, zmm5, zword [0x3c4]
	vptestmb k7, zmm6, zmm0

%ifdef ERROR
	vptestmb k2, xmm8, xmm11
	vptestmb k6, ymm8, ymm14
	vptestmb k7, zmm11, zmm14
	vptestmb k3, xmm20, xmm21
	vptestmb k2, ymm18, ymm23
	vptestmb k5, zmm30, zmm26
%endif
