from dataclasses import dataclass
u:dict[str,list|dict] = {"ins":["LD","ST","PS","PPT","PPK","GNUM","SWP","OUT","ALUCALL","JMP","JEQ","JNE","JGT","JLT","JZR","RET","AI","AO"],
     "hins":[".include",".label",".var",".str"],
     "regs":[r"%ax",r"%bx",r"%cx",r"%dx",r"%ir",r"%pc",r"%zr",r"%fl"],
     "constant":{".mem":0xf0,".stk":0xf1,  # aliasMachine
                ".add":0x00,".sub":0x01,".inc":0x02,".dec":0x03,".mul":0x04,".div":0x05,  # article
                ".and":0x06,".or":0x07,".lst":0x08,".rst":0x09,".xor":0x0a},  # logic
     "address_article_syms":['+','-']
}
@dataclass
class com_obj():
    ins:str
    args:list[str|int]
@dataclass
class hcom_obj():
    hins:str
    info:list[str|int]
    arg:list[str|int]

def lexer(datas:str):
    datas = datas.upper()
    split_datas:list[str] = datas.split(' ')
    tokens = []
    nja = None
    
def objseter(token_lens):
    ...