import csv
import matplotlib.pyplot as plt # for plotting
import numpy as np # for sine function

def Run_Experiment():
    # Analyze_Data('sigA.csv')
    # Analyze_Data('sigB.csv')
    # Analyze_Data('sigC.csv')
    Analyze_Data('sigD.csv')
    


def Analyze_Data (filename):
    # Original Data
    time, data = Extract_Data(filename)
    og_frq, og_Y = FFT(time, data)
    
    # Moving Average Data
    MA_time, MA_data = Moving_Average(time, data)
    ma_freq, ma_Y = FFT(MA_time,MA_data)
    
    # IIR Data
    IIR_time, IIR_data = IIR(time, data, a=0.8, b=0.2)
    IIR_freq, IIR_y = FFT(IIR_time,IIR_data)

    # FIR Data
    FIR_time, FIR_data = FIR(time, data)
    FIR_freq, FIR_y = FFT(FIR_time, FIR_data)
    
    # Plot to compare the moving average data to the original data
    #Plot_Results(time, data, og_frq, og_Y, MA_time, MA_data, ma_freq, ma_Y)

    # Plot to compare the IIR data to the original data
    #Plot_Results(time, data, og_frq, og_Y, IIR_time, IIR_data, IIR_freq, IIR_y)

    # Plot to compare the FIR data to the original data
    Plot_Results(time, data, og_frq, og_Y, FIR_time, FIR_data, FIR_freq, FIR_y)


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
    return frq, Y

def Plot_Results(time_1, data_1, FFT_frq1, FFT_Y1, time_2, data_2, FFT_frq2, FFT_Y2):
    fig, (ax1, ax2) = plt.subplots(2, 1)
    ax1.plot(time_1,data_1,'black', label='unfiltered')
    ax1.plot(time_2, data_2, 'red', label='filtered')
    ax1.set_xlabel('Time')
    ax1.set_ylabel('Amplitude')
    ax1.legend()
    ax2.loglog(FFT_frq1,abs(FFT_Y1),'black', label='unfiltered') # plotting the fft
    ax2.loglog(FFT_frq2,abs(FFT_Y2),'red', label='filtered')
    ax2.set_xlabel('Freq (Hz)')
    ax2.set_ylabel('|Y(freq)|')
    ax2.legend()
    fig.suptitle("Cutoff Frequency 1kHz, Transition Bandwidth 500Hz")
    plt.show()

# Create an array of the moving average of the input data
def Moving_Average(time_vals, data_vals):
    new_time = time_vals
    new_data = []
    x = 10

    for i in range(len(time_vals)):
        average = calc_average(data_vals,i,x)
        new_data.append(average)
    
    return new_time, new_data
    

# Calculate the value for the moving average at each interval step
def calc_average(data, index, x):
    input_x = x
    total = 0
    if index < x:
        return total
    else:
        for i in range(x):
            total = total + data[index-x]
            x = x - 1

    average = total/input_x
    return average

# Create an array of the data with an Infinite Impulse Response Filter
def IIR(time_vals, data_vals, a, b):
    new_time = time_vals
    new_data = []

    for i in range(len(time_vals)):
        if i == 0:
            new_data.append(data_vals[i])
        else:
            average = (new_data[i-1]*a) + (data_vals[i]*b)
            new_data.append(average)

    return new_time, new_data
        

def FIR(time_vals,data_vals):
    new_time = time_vals
    new_data = []

    h = [
    0.000483453019268102,
    -0.000000000000000001,
    -0.000577622798583425,
    -0.001094433343909418,
    -0.001316653869662804,
    -0.000992895648606092,
    0.000000000000000001,
    0.001490674298347418,
    0.002939930829479684,
    0.003560704783606081,
    0.002646609802277267,
    -0.000000000000000003,
    -0.003749123169872504,
    -0.007151929277613168,
    -0.008386477627803779,
    -0.006049864174869907,
    0.000000000000000004,
    0.008157993487561027,
    0.015279549467374122,
    0.017675908704374580,
    0.012646702799170228,
    -0.000000000000000006,
    -0.017101231533321902,
    -0.032468807624358477,
    -0.038497398533104556,
    -0.028648707461908812,
    0.000000000000000007,
    0.045088548451548083,
    0.098908245481145601,
    0.150132541885070386,
    0.186895906340835749,
    0.200256751427113189,
    0.186895906340835749,
    0.150132541885070386,
    0.098908245481145601,
    0.045088548451548083,
    0.000000000000000007,
    -0.028648707461908816,
    -0.038497398533104563,
    -0.032468807624358477,
    -0.017101231533321902,
    -0.000000000000000006,
    0.012646702799170228,
    0.017675908704374584,
    0.015279549467374134,
    0.008157993487561029,
    0.000000000000000004,
    -0.006049864174869905,
    -0.008386477627803779,
    -0.007151929277613172,
    -0.003749123169872504,
    -0.000000000000000003,
    0.002646609802277267,
    0.003560704783606084,
    0.002939930829479684,
    0.001490674298347418,
    0.000000000000000001,
    -0.000992895648606093,
    -0.001316653869662804,
    -0.001094433343909419,
    -0.000577622798583426,
    -0.000000000000000001,
    0.000483453019268102,
    ]
    filter_length = len(h)
    for i in range(len(time_vals)):
        if i <= filter_length:
            new_data.append(data_vals[i])
        else:
            x = filter_length
            filter_value = 0
            for j in range(len(h)):
                    filter_value += (h[j] * data_vals[i-x])
                    x -= 1
            new_data.append(filter_value)

    return new_time, new_data

Run_Experiment()
