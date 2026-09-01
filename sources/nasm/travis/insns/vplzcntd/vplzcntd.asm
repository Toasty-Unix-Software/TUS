default rel
	vplzcntd xmm0, xmm1
	vplzcntd xmm6, xmm2
	vplzcntd ymm7, ymm7
	vplzcntd ymm7, ymm0
	vplzcntd zmm4, zword [0x2af]
	vplzcntd zmm0, zmm1
	vplzcntd xmm14, xmm8
	vplzcntd ymm14, ymm15
	vplzcntd zmm12, zmm14
	vplzcntd xmm17, xmm21
	vplzcntd ymm22, ymm31
	vplzcntd zmm24, zmm31
	vplzcntd xmm5{k6}, xmm5
	vplzcntd ymm0{k4}, yword [0xc58]
	vplzcntd zmm3{k5}, zmm2
	vplzcntd xmm7{k6}{z}, xmm2
	vplzcntd ymm2{k6}{z}, yword [0xedd]
	vplzcntd zmm7{k3}{z}, zmm3
	vplzcntd xmm1, oword [eax+1]
	vplzcntd xmm4, oword [eax+64]
	vplzcntd ymm6, yword [eax+1]
	vplzcntd ymm0, yword [eax+64]
	vplzcntd zmm2, zword [eax+1]
	vplzcntd zmm6, zword [eax+64]
	vplzcntd xmm0, [0xb61]
	vplzcntd ymm6, [0x38d]
	vplzcntd zmm4, [0xbb6]
