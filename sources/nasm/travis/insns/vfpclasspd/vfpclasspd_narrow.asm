	vfpclasspd k6, xmm0, 0xca
	vfpclasspd k3, oword [0xe70], 0xd1
	vfpclasspd k7, yword [0xd60], 0x97
	vfpclasspd k1, ymm4, 0xbd
	vfpclasspd k3, zmm6, 0xc4
	vfpclasspd k4, zword [0x752], 0x95

%ifdef ERROR
	vfpclasspd k3, xmm14, 0x48
	vfpclasspd k1, ymm11, 0x1
	vfpclasspd k6, zmm12, 0xe6
	vfpclasspd k6, xmm19, 0x99
	vfpclasspd k2, ymm20, 0xe
	vfpclasspd k2, zmm29, 0x31
%endif
