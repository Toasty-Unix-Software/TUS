	vpbroadcastq xmm4, qword [0x9f8]
	vpbroadcastq xmm6, qword [0x351]
	vpbroadcastq xmm0, xmm7
	vpbroadcastq xmm5, xmm6
	vpbroadcastq ymm0, qword [0x290]
	vpbroadcastq ymm7, qword [0xa8c]
	vpbroadcastq ymm3, xmm1
	vpbroadcastq ymm4, xmm3

%ifdef ERROR
	vpbroadcastq xmm15, qword [0x3c4]
	vpbroadcastq xmm8, xmm15
	vpbroadcastq ymm9, qword [0x553]
	vpbroadcastq ymm11, xmm10
	vpbroadcastq xmm29, qword [0x70e]
	vpbroadcastq xmm17, xmm26
	vpbroadcastq ymm17, qword [0x5f2]
	vpbroadcastq ymm19, xmm24
%endif
