	kmovd k7, k2
	kmovd k1, k4
	kmovd dword [0x773], k7
	kmovd dword [0x904], k6
	kmovd k5, eax
	kmovd k6, ebp
	kmovd eax, k3
	kmovd ebx, k4

%ifdef ERROR
	kmovd k7, r15d
	kmovd r15d, k4
	kmovd k7, r22d
	kmovd r18d, k3
%endif
