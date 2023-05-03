import csv
import matplotlib.pyplot as plt # for plotting
import numpy as np # for sine function

def Run_Experiment():
    A_time, A_data = Extract_Data('sigA.csv')
    FFT(A_time, A_data)
    B_time, B_data = Extract_Data('sigB.csv')
    FFT(B_time, B_data)
    C_time, C_data = Extract_Data('sigC.csv')
    FFT(C_time, C_data)
    D_time, D_data = Extract_Data('sigD.csv')
    FFT(D_time, D_data)

# Extracting the Data from the CSV Files
def Extract_Data(filename):
    t = [] # column 0
    data1 = [] # column 1
    #data2 = [] # column 2

    with open(filename) as f:
        # open the csv file
        reader = csv.reader(f)
        for row in reader:
            # read the rows 1 one by one
            t.append(float(row[0])) # leftmost column
            data1.append(float(row[1])) # second column
            #data2.append(float(row[2])) # third column

    return t, data1
    # for i in range(len(t)):
    #     # print the data to verify it was read
    #     print(str(t[i]) + ", " + str(data1[i]))# + ", " + str(data2[i]))


# Carrying out the Fast Fourier Transform to the Data to find relevant frequencies
def FFT(time_vals, data_vals):
    dt = 1/int(len(time_vals)/time_vals[-1]) # sampling interval
    t = time_vals # length of the input data
    # a constant plus 100Hz and 1000Hz
    

    Fs = int(len(time_vals)/time_vals[-1]) # sample rate
    Ts = 1.0/Fs; # sampling interval
    ts = np.arange(0,t[-1],Ts) # time vector
    y = data_vals # the data to make the fft from
    n = len(y) # length of the signal
    k = np.arange(n)
    T = n/Fs
    frq = k/T # two sides frequency range
    frq = frq[range(int(n/2))] # one side frequency range
    Y = np.fft.fft(y)/n # fft computing and normalization
    Y = Y[range(int(n/2))]
    length = len(t)

    fig, (ax1, ax2) = plt.subplots(2, 1)
    ax1.plot(t,y,'b')
    ax1.set_xlabel('Time')
    ax1.set_ylabel('Amplitude')
    ax2.loglog(frq,abs(Y),'b') # plotting the fft
    ax2.set_xlabel('Freq (Hz)')
    ax2.set_ylabel('|Y(freq)|')
    plt.show()

def Moving_Average(time_vals, data_vals):
    
    return

Run_Experiment()
