import numpy as np # to get access to ulab numpy functions
import matplotlib.pyplot as plt



start_time = 0
end_time = 1
sample_rate = 1024

time = np.arange(start_time, end_time, 1/sample_rate)


theta_1 = 0
freq_1 = 2
ampl_1 = 1
sinewave_1 = ampl_1 * np.sin(2 * np.pi * freq_1 * time + theta_1)

theta_2 = 0
freq_2 = 5
ampl_2 = 0.25
sinewave_2 = ampl_2 * np.sin(2 * np.pi * freq_2 * time + theta_2)

theta_3 = 0
freq_3 = 50
ampl_3 = 0.1
sinewave_3 = ampl_3 * np.sin(2 * np.pi * freq_3 * time + theta_3)


sinewave_total = sinewave_1 + sinewave_2 + sinewave_3
fft = np.fft.fft(sinewave_total)

N = len(fft)
n = np.arange(N)
T = N/sample_rate
fft_freq = n/T

# plt.plot(sinewave_1)
# plt.show()

# plt.plot(sinewave_2)
# plt.show()

# plt.plot(sinewave_3)
# plt.show()

plt.plot(sinewave_total)
plt.show()

plt.plot(fft_freq, abs(fft))
plt.xlim(0,75)
plt.show()