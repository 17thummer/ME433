# get a line of raw bitmap and plot the components
import serial
ser = serial.Serial('COM9',230400) # the name of your Pico port
print('Opening port: ')
print(ser.name)

ser.write(b'hi\r\n') # send a newline to request data
data_read = ser.read_until(b'\n',50) # read the echo

sampnum = 0
index = 0
raw = []
reds = []
greens = []
blues = []
bright = []


# Function for finding the COM of the line based on the red, green and blue input values from image
def bw_image(red, green, blue):
    filtered = []

    #inverting the data... high values = black areas in the image
    for i in range(len(red)):
        if (red[i] + green[i] + blue[i]) > 175:
            filtered.append(0)
        else:
            filtered.append(100)
    
    return filtered

def grayscale(red, green, blue, bright):
    filtered = []
    
    #inverting the data... high values = black areas in the image
    for i in range(len(red)):
        gray = (red[i] + green[i] + blue[i])/3
        filtered.append(gray)
    
    return filtered


def find_COM(filtered_data):
    numerator = 0
    denominator = 0

    for i in range(len(filtered_data)):
        numerator += (filtered_data[i] * i)
        denominator += filtered_data[i]
    
    COM = round(numerator/denominator)
    return COM

# Pico sends back index and raw pixel value
while sampnum < 60: # width of bitmap
    data_read = ser.read_until(b'\n',50) # read until newline
    data_text = str(data_read,'utf-8') # convert bytes to string
    data = list(map(int,data_text.split())) # convert string to values

    if(len(data)==2):
        index = data[0]
        raw.append(data[1])
        reds.append(((data[1]>>5)&0x3F)/0x3F*100) # red value is middle 6 bits
        greens.append((data[1]&0x1F)/0x1F*100) # green value is rightmost 5 bits
        blues.append(((data[1]>>11)&0x1F)/0x1F*100) # blue vale is leftmost 5 bits
        bright.append((data[1]&0x1F)+((data[1]>>5)&0x3F)+((data[1]>>11)&0x1F)) # sum of colors
        sampnum = sampnum + 1

# print the raw color as a 16bit binary to double check bitshifting
for i in range(len(reds)):
    print(f"{raw[i]:#018b}")

filtered_image = grayscale(reds, blues, greens, bright)
com = find_COM(filtered_image)
print(com)

# plot the colors 
import matplotlib.pyplot as plt 
x = range(len(reds)) # time array
plt.plot(x,reds,'r*-',x,greens,'g*-',x,blues,'b*-', x,filtered_image,'k*-')
plt.ylabel('color')
plt.xlabel('position')
plt.show()

# be sure to close the port
ser.close()


