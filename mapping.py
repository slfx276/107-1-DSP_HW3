Big5_ZhuYin = open("Big5-ZhuYin.map" , 'r' , encoding = 'big5-hkscs')
data = Big5_ZhuYin.readlines()
Big5_ZhuYin.close()

# preprocess Big5-ZhuYin.map
for i in range(len(data)):
    data[i] = data[i].strip('\n ')
    temp = set()
    for j in range(len(data[i])):
        if j != 0:
            if data[i][j-1] == ' ' or data[i][j-1] == '/':
                temp.add(data[i][j])
    data[i] = data[i][0]
    for item in temp:
        data[i] = data[i] + item

# Create dictionary of ZhuYin-Big5.map
map_dic = {}
for i in range(len(data)):
    for j in range(1 , len(data[i])):
        if not data[i][j] in map_dic.keys():
            map_dic[data[i][j]] = []
        # add big5 character to dic
        map_dic[data[i][j]].append(data[i][0])

# write in to ZhuYin-Big5.map
ZhuYin_Big5 = open("ZhuYin-Big5.map" , 'w' , encoding = 'big5-hkscs')
# print mapping
for key in map_dic.keys():
    ZhuYin_Big5.write(key + "\t" )
    for num in range(len(map_dic[key])):
        ZhuYin_Big5.write(map_dic[key][num] )
        if num == len(map_dic[key])-1:
            ZhuYin_Big5.write('\n')
        else:
            ZhuYin_Big5.write(' ')
# print char to char
for i in range(len(data)):
    ZhuYin_Big5.write(data[i][0] + '\t' + data[i][0] + '\n')

ZhuYin_Big5.close()