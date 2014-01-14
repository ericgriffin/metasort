import csv
file = open("Generic_c_array.txt", "w")
with open('Generic.csv', 'rb') as csvfile:
    for row in csvfile:
        a = row.split(";;;")
        file.write("{ \"")
        try:
            file.write(a[0])
        except:
            file.write(" ")
        file.write("\", \"")
        try:

            if(a[1][-1:] == "\n"):
                a[1] = a[1][:-1]
            if(a[1][-1:] == "\r"):
                a[1] = a[1][:-1]
            file.write(a[1])
        except:
            file.write(" ")
        file.write("\", \"")
        try:
            file.write(a[2][:-2])
        except:
            file.write(" ")
        file.write("\" },\n")

file.close()

file = open("General_c_array.txt", "w")
with open('General.csv', 'rb') as csvfile:
    for row in csvfile:
        a = row.split(";;;")
        file.write("{ \"")
        try:
            file.write(a[0])
        except:
            file.write(" ")
        file.write("\", \"")
        try:

            if(a[1][-1:] == "\n"):
                a[1] = a[1][:-1]
            if(a[1][-1:] == "\r"):
                a[1] = a[1][:-1]
            file.write(a[1])
        except:
            file.write(" ")
        file.write("\", \"")
        try:
            file.write(a[2][:-2])
        except:
            file.write(" ")
        file.write("\" },\n")

file.close()


file = open("Audio_c_array.txt", "w")
with open('Audio.csv', 'rb') as csvfile:
    for row in csvfile:
        a = row.split(";;;")
        file.write("{ \"")
        try:
            file.write(a[0])
        except:
            file.write(" ")
        file.write("\", \"")
        try:

            if(a[1][-1:] == "\n"):
                a[1] = a[1][:-1]
            if(a[1][-1:] == "\r"):
                a[1] = a[1][:-1]
            file.write(a[1])
        except:
            file.write(" ")
        file.write("\", \"")
        try:
            file.write(a[2][:-2])
        except:
            file.write(" ")
        file.write("\" },\n")

file.close()


file = open("Video_c_array.txt", "w")
with open('Video.csv', 'rb') as csvfile:
    for row in csvfile:
        a = row.split(";;;")
        file.write("{ \"")
        try:
            file.write(a[0])
        except:
            file.write(" ")
        file.write("\", \"")
        try:

            if(a[1][-1:] == "\n"):
                a[1] = a[1][:-1]
            if(a[1][-1:] == "\r"):
                a[1] = a[1][:-1]
            file.write(a[1])
        except:
            file.write(" ")
        file.write("\", \"")
        try:
            file.write(a[2][:-2])
        except:
            file.write(" ")
        file.write("\" },\n")

file.close()


file = open("Text_c_array.txt", "w")
with open('Text.csv', 'rb') as csvfile:
    for row in csvfile:
        a = row.split(";;;")
        file.write("{ \"")
        try:
            file.write(a[0])
        except:
            file.write(" ")
        file.write("\", \"")
        try:

            if(a[1][-1:] == "\n"):
                a[1] = a[1][:-1]
            if(a[1][-1:] == "\r"):
                a[1] = a[1][:-1]
            file.write(a[1])
        except:
            file.write(" ")
        file.write("\", \"")
        try:
            file.write(a[2][:-2])
        except:
            file.write(" ")
        file.write("\" },\n")

file.close()

file = open("Other_c_array.txt", "w")
with open('Other.csv', 'rb') as csvfile:
    for row in csvfile:
        a = row.split(";;;")
        file.write("{ \"")
        try:
            file.write(a[0])
        except:
            file.write(" ")
        file.write("\", \"")
        try:

            if(a[1][-1:] == "\n"):
                a[1] = a[1][:-1]
            if(a[1][-1:] == "\r"):
                a[1] = a[1][:-1]
            file.write(a[1])
        except:
            file.write(" ")
        file.write("\", \"")
        try:
            file.write(a[2][:-2])
        except:
            file.write(" ")
        file.write("\" },\n")

file.close()


file = open("Menu_c_array.txt", "w")
with open('Menu.csv', 'rb') as csvfile:
    for row in csvfile:
        a = row.split(";;;")
        file.write("{ \"")
        try:
            file.write(a[0])
        except:
            file.write(" ")
        file.write("\", \"")
        try:

            if(a[1][-1:] == "\n"):
                a[1] = a[1][:-1]
            if(a[1][-1:] == "\r"):
                a[1] = a[1][:-1]
            file.write(a[1])
        except:
            file.write(" ")
        file.write("\", \"")
        try:
            file.write(a[2][:-2])
        except:
            file.write(" ")
        file.write("\" },\n")

file.close()



file = open("Image_c_array.txt", "w")
with open('Image.csv', 'rb') as csvfile:
    for row in csvfile:
        a = row.split(";;;")
        file.write("{ \"")
        try:
            file.write(a[0])
        except:
            file.write(" ")
        file.write("\", \"")
        try:

            if(a[1][-1:] == "\n"):
                a[1] = a[1][:-1]
            if(a[1][-1:] == "\r"):
                a[1] = a[1][:-1]
            file.write(a[1])
        except:
            file.write(" ")
        file.write("\", \"")
        try:
            file.write(a[2][:-2])
        except:
            file.write(" ")
        file.write("\" },\n")

file.close()
