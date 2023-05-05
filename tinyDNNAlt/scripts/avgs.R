#!/usr/bin/Rscript
#float_times <- scan("./plot/net_float_lr_5_ep_30_mb_4_nt_60000.model/test_perf_float_times.txt",quiet=TRUE);
float_times <- scan("./plot/cf_test_perf_float_times.txt",quiet=TRUE);
#posit16_times <- scan("./plot/net_float_lr_5_ep_30_mb_4_nt_60000.model/test_perf_posit_16_2_times.txt",quiet=TRUE);
#posit14_times <- scan("./plot/net_float_lr_5_ep_30_mb_4_nt_60000.model/test_perf_posit_14_2_times.txt",quiet=TRUE);
#posit12_times <- scan("./plot/net_float_lr_5_ep_30_mb_4_nt_60000.model/test_perf_posit_12_2_times.txt",quiet=TRUE);
#posit12_times <- scan("./plot/net_float_lr_5_ep_30_mb_4_nt_60000.model/test_perf_posit_12_0_times.txt",quiet=TRUE);
#posit10_times <- scan("./plot/net_float_lr_5_ep_30_mb_4_nt_60000.model/test_perf_posit_10_0_times.txt",quiet=TRUE);
#posit10t_times <- scan("./plot/net_float_lr_5_ep_30_mb_4_nt_60000.model/test_perf_posit_10_0_tab_times.txt",quiet=TRUE);
#posit8_times <- scan("./plot/net_float_lr_5_ep_30_mb_4_nt_60000.model/test_perf_posit_8_0_times.txt",quiet=TRUE);
#posit8t_times <- scan("./plot/net_float_lr_5_ep_30_mb_4_nt_60000.model/test_perf_posit_8_0_tab_times.txt",quiet=TRUE);
#posit8t_times <- scan("./plot/test_perf_posit_8_0_tab_times.txt",quiet=TRUE);

avgs <- function(time_vec) {
	avg = mean(time_vec);
	std = sd(time_vec);
	nnn = qnorm(0.99);
	bnd = nnn*std/sqrt(length(time_vec));
	return(c(avg, bnd))
}

c("float",avgs(float_times));
#c("p16,2",avgs(posit16_times));
#c("p14,2",avgs(posit14_times));
#c("p12,2",avgs(posit12_times));
#c("p10,0",avgs(posit10_times));
#c("p10t,0",avgs(posit10t_times));
#c("p8,0",avgs(posit8_times));
#c("p8t,0",avgs(posit8t_times));
