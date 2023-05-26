
import board
from ulab import numpy as np # to get access to ulab numpy functions
import time

start_time = 0
end_time = 1
sample_rate = 1024

time_vals = np.arange(start_time, end_time, 1/sample_rate)


theta_1 = 0
freq_1 = 5
ampl_1 = 1
sinewave_1 = ampl_1 * np.sin(2 * np.pi * freq_1 * time_vals + theta_1)

theta_2 = 0
freq_2 = 25
ampl_2 = 0.25
sinewave_2 = ampl_2 * np.sin(2 * np.pi * freq_2 * time_vals + theta_2)

theta_3 = 0
freq_3 = 50
ampl_3 = 0.1
sinewave_3 = ampl_3 * np.sin(2 * np.pi * freq_3 * time_vals + theta_3)

sinewave_total = sinewave_1 + sinewave_2 + sinewave_3

real, transformed = np.fft.fft(time_vals, sinewave_total)
N = len(transformed)

n = np.arange(N)
T = N/sample_rate
freq = n/T


for i in range(len(sinewave_total)):
    #print("(" + str(np.abs(transformed[i]) + ",)")
    time.sleep(0.02)
    #print(str(sinewave_total[i]), fft)
    print("(" + str(sinewave_total[i]) + ",)")
   # if i < 1023:
   #     print("(" + str(sinewave_total[i]) + ",)")
   #     i += 1

   # else:
   #     i = 0

