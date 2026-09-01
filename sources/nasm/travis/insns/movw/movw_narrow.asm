	movw k7, k3
	movw k5, k7
	movw word [0x964], k4
	movw word [0x240], k4
	movw k3, ebp
	movw k2, edx
	movw k3, di
	movw k2, bp

%ifdef ERROR
	movw k1, r10d
	movw k3, r9w
	movw k7, r18d
	movw k6, r20w
%endif
