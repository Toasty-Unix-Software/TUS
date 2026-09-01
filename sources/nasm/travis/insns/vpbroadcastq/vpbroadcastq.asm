default rel
	vpbroadcastq xmm4, qword [0x9f8]
	vpbroadcastq xmm6, qword [0x351]
	vpbroadcastq xmm0, xmm7
	vpbroadcastq xmm5, xmm6
	vpbroadcastq ymm0, qword [0x290]
	vpbroadcastq ymm7, qword [0xa8c]
	vpbroadcastq ymm3, xmm1
	vpbroadcastq ymm4, xmm3
	vpbroadcastq xmm15, qword [0x3c4]
	vpbroadcastq xmm8, xmm15
	vpbroadcastq ymm9, qword [0x553]
	vpbroadcastq ymm11, xmm10
	vpbroadcastq xmm29, qword [0x70e]
	vpbroadcastq xmm17, xmm26
	vpbroadcastq ymm17, qword [0x5f2]
	vpbroadcastq ymm19, xmm24
	vpbroadcastq xmm6{k6}, qword [0xc50]
	vpbroadcastq ymm0{k7}, qword [0xdbf]
	vpbroadcastq zmm2{k2}, qword [0x4dc]
	vpbroadcastq xmm1{k5}, xmm4
	vpbroadcastq ymm7{k6}, xmm7
	vpbroadcastq zmm1{k6}, xmm3
	vpbroadcastq xmm6{k4}, rdi
	vpbroadcastq ymm1{k3}, rbp
	vpbroadcastq zmm0{k1}, rdi
	vpbroadcastq xmm2{k4}{z}, qword [0xe92]
	vpbroadcastq ymm4{k7}{z}, qword [0xbc7]
	vpbroadcastq zmm1{k5}{z}, qword [0xdc9]
	vpbroadcastq xmm4{k2}{z}, xmm5
	vpbroadcastq ymm6{k5}{z}, xmm5
	vpbroadcastq zmm7{k3}{z}, xmm6
	vpbroadcastq xmm7{k1}{z}, rbp
	vpbroadcastq ymm6{k5}{z}, rbx
	vpbroadcastq zmm4{k3}{z}, rcx
	vpbroadcastq xmm2, qword [eax+1]
	vpbroadcastq xmm0, qword [eax+64]
	vpbroadcastq ymm2, qword [eax+1]
	vpbroadcastq ymm0, qword [eax+64]
	vpbroadcastq xmm5, qword [eax+1]
	vpbroadcastq xmm4, qword [eax+64]
	vpbroadcastq ymm6, qword [eax+1]
	vpbroadcastq ymm1, qword [eax+64]
	vpbroadcastq zmm4, qword [eax+1]
	vpbroadcastq zmm2, qword [eax+64]
	vpbroadcastq xmm0, [0x7a8]
	vpbroadcastq ymm4, [0x296]
	vpbroadcastq xmm4, [0x7f3]
	vpbroadcastq ymm4, [0x992]
	vpbroadcastq zmm5, [0xbca]
