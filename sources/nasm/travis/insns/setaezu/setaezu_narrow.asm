	setaezu ecx
	setaezu ecx
	setaezu byte [0xa1e]
	setaezu bl

%ifdef ERROR
	setaezu rcx
	setaezu rbx
	setaezu r9
	setaezu r12d
	setaezu r11b
	setaezu r31
	setaezu r20d
	setaezu r22b
%endif
