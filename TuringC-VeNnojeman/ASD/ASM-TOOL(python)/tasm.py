import sys
AFP = sys.argv[1]
OFP = sys.argv[2]

keyword_list = ["func","funcend","include","callfunc"]
control_list = ["Ld","St","Ps","Ppt","Ppk","Gnum","Swp","Out","ALUCall","Jmp","Jeq","Jne","Jgt","Jlt","Jzr","Ret","AI","AO"]
register_list= [r"%ax",r"%bx",r"%cx",r"%dx",r"%ir",r"%pc",r"%zr",r"%fl"]
magicnum_map = {".mem":0xf0,".stk":0xf1,  # aliasMachine
                ".add":0x00,".sub":0x01,".inc":0x02,".dec":0x03,".mul":0x04,".div":0x05,  # article
                ".and":0x06,".or":0x07,".lst":0x08,".rst":0x09,".xor":0x0a}  # logic
control_map  = dict()
register_map = dict()
def map_init():
    global control_map,register_map
    for i in range(1,19):
        control_map[control_list[i-1]] = i
    for j in range(0,8):
        register_map[register_list[j]] = j
    return None
def read_file(InputFilePath:str):
    content = []
    with open(InputFilePath,mode="r",encoding="utf-8") as IF:
        content = IF.readlines()
    for i in range(len(content)):
        content[i] = content[i][:-1].split(" ")
        #                      ^^^^^
        #清理换行符
    return content
def read_token(InputTokens:list[list[str]]):
    global token_list,function_map
    token_list = []
    function_map = {}
    funcFlag = 0
    funcName = ""
    writeto = None
    for i in range(len(InputTokens)):
        if funcFlag:
            writeto = function_map[funcName]
        else:
            writeto = token_list
        for j in range(len(InputTokens[i])):
            nja = InputTokens[i][j]
            if len(nja) == 0:continue
            if(nja[0] == ";"):
                break
            elif(nja[0] == "$"):
                writeto.append((nja,"address"))
            elif (nja in control_list) and (j == 0) :
                writeto.append((nja,"control"))
            elif (nja in register_list) and (j != 0) :
                writeto.append((nja,"register"))
            elif (nja in magicnum_map.keys()) and (j != 0) :
                writeto.append((nja,"magicnum"))
            elif (nja in keyword_list) :
                match nja:
                    case "include":
                        writeto.append((InputTokens[i][j+1],"include"))
                        break
                    case "func":
                        funcFlag = 1
                        funcName = InputTokens[i][j+1]
                        function_map[funcName] = []
                        break
                    case "funcend":
                        funcFlag = 0
                        funcName = None
                    case "callfunc":
                        writeto.append((InputTokens[i][j+1],"link"))
                        break
            elif (nja[:2] == "0x"):
                writeto.append((nja,"hexnum"))

            else:
                writeto.append((nja,"AnyErr"))
        writeto.append("\n")
    return token_list,function_map
def linker(token_list:list[tuple[str]],function_map:dict):
    for i in range(len(token_list)):
        nja = token_list[i]
        if nja == "\n":
            continue
        if nja[1] == "include":
            if nja[0][0] == '<':
                # standard moudule
                path = ".\\Lib\\"+nja[0][1:-1]
                funcl = read_token(read_file(path))[1]
                function_map.update({nja[0][1:-3]:funcl})
            else:
                path = ".\\"+nja[0]
                funcl = read_token(read_file(path))[1]
                function_map.update({nja[0][:-3]:funcl})
        elif nja[1] == "link":
            mouduleName,funcName = nja[0].split('.')
            function = function_map[mouduleName][funcName]
            token_list[i:i+1] = function
        # elif nja[1] == "AnyErr":
        #     raise "Grammar or name error"
        else:
            pass
    return token_list,function_map
def split_list(data, split_element):
    result = []
    temp_list = []

    for element in data:
        if element == split_element:
            if temp_list:
                result.append(temp_list)
            temp_list = [element]
        else:
            temp_list.append(element)

    if temp_list:
        result.append(temp_list)

    return result
def binner(token_list):
    out_list = []
    line_list = split_list(token_list,'\n')
    for i in range(len(line_list)):
        out_list.append([])
        for j in range(len(line_list[i])):
            nja = line_list[i][j]
            if nja == '\n':
                continue
            if nja[1] == "include":
                continue
            elif nja[1] == "control":
                out_list[i].append(control_map[nja[0]])
            elif nja[1] == "register":
                out_list[i].append(register_map[nja[0]])
            elif nja[1] == "hexnum":
                out_list[i].append(int(nja[0][2:],base=16))
            elif nja[1] == "magicnum":
                out_list[i].append(magicnum_map[nja[0]])
            elif nja[1] == "address":
                out_list[i].append(eval(str(i)+nja[0][1:]))
            else:
                pass
        out_list[i] = out_list[i]+[0]*(4-len(out_list[i]))
    return out_list
def writer(ol):
    hexl = []
    for i in ol:
        hexl.append(bytearray(i))
    with open(OFP,mode="wb+") as f:
        f.writelines(hexl)

def test():
    map_init()
    eefile = read_file(AFP)
    tokenl,funcl = read_token(eefile)
    ltokenl,lfuncl = linker(tokenl,funcl)
    ol = binner(ltokenl)
    writer(ol)
    print("file:\n",eefile,"\n","tokens:\n",tokenl,"\n","functions:\n",funcl,"\n","link-tokens:\n",ltokenl,"\n","bytes(dec):\n",ol,sep = None)


if __name__ == "__main__":
    test()