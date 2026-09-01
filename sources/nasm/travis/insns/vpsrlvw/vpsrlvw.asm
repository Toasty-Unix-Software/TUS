default rel
	vpsrlvw xmm1, xmm6, xmm2
	vpsrlvw xmm1, xmm2
	vpsrlvw xmm0, xmm5, xmm1
	vpsrlvw ymm2, ymm0, ymm2
	vpsrlvw ymm2, ymm2
	vpsrlvw ymm7, ymm1, ymm6
	vpsrlvw zmm3, zmm5, zmm2
	vpsrlvw zmm3, zmm2
	vpsrlvw zmm1, zmm5, zword [0xe81]
	vpsrlvw xmm12, xmm8, xmm11
	vpsrlvw ymm14, ymm11, ymm14
	vpsrlvw zmm8, zmm10, zmm10
	vpsrlvw xmm30, xmm17, xmm29
	vpsrlvw ymm31, ymm28, ymm17
	vpsrlvw zmm23, zmm30, zmm20
	vpsrlvw xmm6{k5}, xmm1, xmm3
	vpsrlvw ymm7{k1}, ymm6, ymm2
	vpsrlvw zmm7{k3}, zmm5, zmm7
	vpsrlvw xmm1{k3}{z}, xmm5, oword [0xeca]
	vpsrlvw ymm4{k3}{z}, ymm2, ymm7
	vpsrlvw zmm5{k7}{z}, zmm3, zmm4
	vpsrlvw xmm1, xmm6, oword [eax+1]
	vpsrlvw xmm7, xmm3, oword [eax+64]
	vpsrlvw ymm2, ymm6, yword [eax+1]
	vpsrlvw ymm0, ymm6, yword [eax+64]
	vpsrlvw zmm6, zmm7, zword [eax+1]
	vpsrlvw zmm3, zmm3, zword [eax+64]
	vpsrlvw xmm0, xmm4, [0x5ff]
	vpsrlvw ymm6, ymm0, [0x473]
	vpsrlvw zmm0, zmm2, [0xc34]
