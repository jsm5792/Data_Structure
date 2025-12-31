#include <iostream>
#include <string>
using namespace std;
// 변경 없음

unsigned int StringToInt (string s)     // hash function
{
    int length = (int) s.length();
    unsigned int answer = 0;
    if (length % 2 == 1)
    {
        answer = s.at(length-1);
        length--;
    }
    for(int i=0;i<length;i+=2)
    {
        answer+=s.at(i);
        answer+=((int)s.at(i+1))<<8;
    }
    return answer;
}

string lower(string &s)                 // stirng 소문자로 저장
{
    for(char &i: s)
    {
        if(i>='A' && i<= 'Z')
        {
            i+=32;
        }
    }
    return s;
}


struct sNode{
    string key;
    int value;
    sNode* next;
    sNode(string k, int v) : key(k), value(v), next(nullptr) {}

};

sNode* stackTop = nullptr;              // stack의 top을 나타내는 변수

class NodeArray{
public:
    void setLNode(int x)
    {
        this->lchild = x;
    }
    void setRNode(int x)
    {
        this->rchild = x;
    }
    int getLNode()
    {
        return this->lchild;
    }
    int getRNode()
    {
        return this->rchild;
    }

protected:
    int lchild=0;
    int rchild=0;
};

class hashTable{

public:
    void SetHash(string s)
    {
        this->hashvalue = -StringToInt(s);
        this->symbol = lower(s);
    }
    void SetHash2(string s, int i)
    {
        this->hashvalue = i;
        this->symbol = lower(s);
    }
    int gethvalue()
    {
        return this->hashvalue;
    }
    string getsymbol()
    {
        return this->symbol;
    }
    int getlinkvalue()
    {
        return this->linkofvalue;
    }
    void setlinkvalue(int i)
    {
        this->linkofvalue = i;
    }
protected:
    int hashvalue=0;
    string symbol="";
    int linkofvalue=0;
};

bool IsNumber(string s)                 // string이 숫자인지 판별
{
    for(char c : s)
    {
        if(!isdigit(c))
        {
            return false;
        }
    }
    return true;
}


pair<string,string> getNextToken(string s)              // token 분리해주는 함수
{
    string result = "";
    string temp = "";
    int count = 0;
    if(s=="")
    {
        return {"",""};
    }
    while(true)
    {
        if(s[count]=='(')
        {
            if(temp=="")
            {
                result += s[count];
                return {result, s.substr(count+1)};                
            }
            else
            {
                result += temp;
                return {result, s.substr(count)};
            }
        }
        else if(s[count]==' ')
        {
            if(temp=="")
            {
                count++;
            }
            else
            {
                result += temp;
                return {result, s.substr(count)};
            }

        }
        else if(s[count]==')')
        {
            if(temp=="")
            {
                result += s[count];
                return {result, s.substr(count+1)};
            }
            else
            {
                result += temp;
                return {result, s.substr(count)};
            }
        }
        else if(s[count]=='\'')
        {
            if(temp!="")
            {
                result += temp;
                return {result, s.substr(count)};
            }
            else
            {
                result += s[count];
                return {result, s.substr(count+1)};
            }
        }
        else
        {
            temp += s[count];
            count ++;
        }

    }
}

string Preprocessing(string s)           // 처음의 string eval 편하게 변경해주는 함수
{
    string result = "";
    string token = getNextToken(s).first;
    s = getNextToken(s).second;
    while(token != "")
    {
        if(token == "define")
        {
            result += "define ";
            token = getNextToken(s).first;
            s = getNextToken(s).second;
            if(token == "(")
            {
                token = getNextToken(s).first;
                s = getNextToken(s).second;
                result += token;
                result += " ";
                result += "(lambda (";
                result += Preprocessing(s);
                result += ")";
                return result;
            }
            else
            {
                result += token;
                result += " ";
                result += Preprocessing(s);
                return result;
            }
        }
        else if(token =="\'")
        {
            result += "(quote ";
            int num = 0;
            do{
                token = getNextToken(s).first;
                s = getNextToken(s).second;
                result += token;
                result += " ";
                if(token =="(")
                {
                    num++;
                }
                else if(token ==")")
                {
                    num--;
                }
            } while (num > 0);
            result += ")";
            token = getNextToken(s).first;
            s = getNextToken(s).second;
        }
        else
        {
            result += token;
            if(token !="(" && token != ")")
            {
                result += " ";
            }
            token = getNextToken(s).first;
            s = getNextToken(s).second;
        }
    }
    return result;
    
}


int isinit(hashTable *table, string s,int size)         // hashTable 안에 문자열이 이미 있는지 확인
{
    int j = -StringToInt(lower(s));
    for(int i=0; i<101; i++)
    {
        if(table[i].getsymbol() == s)
        {
            return 1;                   // 1 이면 symbol 존재
        }
    }
    for(int i=0; i<101; i++)
    {
        if(table[i].gethvalue()== j)
        {
            return 2;                   // 2면 symbol은 없는데 같은 hvalue 갖는 값 존재
        }
    }
    return 0;
}


bool isinit(hashTable *table, int value)                // hashTable 안에 value가 있는지 확인
{
    for(int i=0;i<101;i++)
    {
        if(table[i].gethvalue() == value)
        {
            return true;
        }
    }
    return false;
}

void pushTable(hashTable *table, string s, int value)   // hashTable에 string, value push하는 함수
{
    int i=0;
    while(i<101)
    {
        if(table[(value+i)%101].gethvalue()==0 && table[(value+i)%101].getsymbol()=="")
        {
            table[(value+i)%101].SetHash2(s,-value);
            break;
        }
        i++;
    }
    return;
}

void pushTable2(hashTable *table, string s)             // hashTable에 string 자동으로 넣어주는 함수
{
    if(isinit(table,s,101)==1)          //이미 있는 경우
    {
        return;
    }
    else if(isinit(table,s,101)==2)
    {
        int j=1;
        while(1)
        {
            if(!isinit(table,-(StringToInt(lower(s))+j)))
            {
                break;
            }
            j++;
        }
        pushTable(table,lower(s),StringToInt(lower(s))+j);
        return;
    }
    else
    {
        pushTable(table, s,StringToInt(lower(s)));
        return;
    }
}

void pushTablelink(hashTable *table, int key, int value)        // key에 해당하는 linkofvalue 변경 (key는 음수 상태)
{
    int i=0;
    while(i<101)
    {
        if(table[i].gethvalue()==key)
        {
            table[i].setlinkvalue(value);
            break;
        }
        i++;
    }
    return;
}

void pushTablelink(hashTable *table, string s, int value)
{
    int i=0;
    while(i<101)
    {
        if(table[i].getsymbol()==s)
        {
            table[i].setlinkvalue(value);
            break;
        }
        i++;
    }
    return;
}

void pushStack(string key, int value)                   // stack에 push
{
    if(stackTop == nullptr)
    {
        sNode* newNode = new sNode(key,value);
        newNode->next = nullptr;
        stackTop = newNode;
    }
    else
    {
        sNode* newNode = new sNode(key,value);
        newNode->next = stackTop;
        stackTop = newNode;
    }
}

void popStack()                                         // stack에서 pop
{
    if(stackTop == nullptr)
    {
        cout << "Error";
    }
    else
    {
        stackTop = stackTop->next;
    }

}

int check = 0; // 괄호 두번 확인하는 용도 (read 함수에서 사용)
int hashtable_size = 0; // hashtable 관리
int getHashvalue(hashTable *,string);
int nodearray_size = 0;

pair<int,int> read(int node_num,int string_num,string s,NodeArray *array,hashTable *table)      // 읽어주는 함수
{
    int temp1;
    if(string_num>(int)s.length()-1)
    {
        return {node_num,string_num};
    }
    if(s[string_num]=='(')
    {
        int n = string_num+1;
        string temp = "";
        int x = node_num;
        while(true)
        {
            if(s[n]=='(')
            {
                if(check == 1)
                {
                    array[x].setRNode(node_num);
                    check = 0;
                }
                x = node_num;
                temp1 = node_num;
                node_num++;
                array[temp1].setLNode(node_num);
                auto result = read(node_num,n,s,array,table);
                node_num = result.first;
                n = result.second;
                check=1;
            }
            else if(s[n]==')')
            {
                if(temp!="")
                {
                    if(isinit(table,lower(temp),101)==0)
                    {
                        pushTable(table,lower(temp),StringToInt(lower(temp)));
                        array[node_num].setLNode(-StringToInt(lower(temp)));
                        node_num++;
                    }
                    else if(isinit(table,lower(temp),101)==2)
                    {
                        int j=1;
                        while(1)
                        {
                            if(!isinit(table,-(StringToInt(lower(temp))+j)))
                            {
                                break;
                            }
                            j++;
                        }
                        pushTable(table,lower(temp),StringToInt(lower(temp))+j);
                        array[node_num].setLNode(-(StringToInt(lower(temp))+j));
                        node_num++;
                    }
                    else
                    {
                        array[node_num].setLNode(getHashvalue(table,lower(temp)));
                        node_num++;
                    }
                    array[node_num].setRNode(0);
                    temp="";
                }
                else
                {
                    if(node_num==x)
                    {
                        array[node_num-1].setRNode(0);
                        node_num++;
                    }
                    else
                    {
                        array[node_num-1].setRNode(0);
                    }
                    
                }
                break;
            }
            else if(s[n]==' ')
            {
                if(temp!="")
                {
                    if(check==1)
                    {
                        array[x].setRNode(node_num);
                        check = 0;
                    }
                    if(isinit(table,lower(temp),101)==0)
                    {
                        pushTable(table,lower(temp),StringToInt(lower(temp)));
                        array[node_num].setLNode(-StringToInt(lower(temp)));
                    }
                    else if(isinit(table,lower(temp),101)==2)
                    {
                        int j=1;
                        while(1)
                        {
                            if(!isinit(table,-(StringToInt(lower(temp))+j)))
                            {
                                break;
                            }
                            j++;
                        }
                        pushTable(table,lower(temp),StringToInt(lower(temp))+j);
                        array[node_num].setLNode(-(StringToInt(lower(temp))+j));
                    }
                    else
                    {
                        array[node_num].setLNode(getHashvalue(table,lower(temp)));
                    }
                    temp1 = node_num;
                    node_num++;
                    array[temp1].setRNode(node_num);
                    
                    temp="";
                }
            }
            else
            {
                temp+=s[n];
            }
            n++;

        }
        string_num=n;
    }
    return {node_num,string_num};
}

void printNode(NodeArray *array,int i)          // NodeArray  출력 함수
{
    int j=1;
    while(j<i)
    {
        /*
        if(array[j].getLNode()==0 && array[j].getRNode()==0)
        {
            break;
        }
        */
        cout << "Node ID: " << j << " / left: " << array[j].getLNode() << " / right: " <<array[j].getRNode()<<endl;
        j++;
    }
}

void printHash(hashTable *table,int size)       // Hashtable 출력 함수
{
    for(int i=0;i<101;i++)
    {
        if(table[i].gethvalue()!=0 && table[i].getsymbol()!="")
        {
            cout << "Hashvalue: " << table[i].gethvalue() << " / symbol: " << table[i].getsymbol();
            cout << " / link of value: " << table[i].getlinkvalue() <<endl;
        }
    }
}

void sortHash(hashTable *table,int size)        // Hashtable 정렬 함수
{
    for(int i=0;i<size-1;i++)
    {
        for(int j=0;j<size-1-i;j++)
        {
            if(table[j].gethvalue() < table[j+1].gethvalue())
            {
                hashTable temp = table[j];
                table[j] = table[j+1];
                table[j+1] = temp;
            }
        }
    }
}

string getHashsymbol(hashTable *table, int value) // hashvalue에 맞는 symbol 반환 함수
{
    for(int i=0;i<101;i++)
    {
        if(table[i].gethvalue()==value)
        {
            return table[i].getsymbol();
        }
    }
    return "";
}

int getHashvalue(hashTable *table, string s)        //string에 맞는 hashvalue 반환 함수
{
    for(int i=0;i<101;i++)
    {
        if(table[i].getsymbol()==s)
        {
            return table[i].gethvalue();
        }
    }
    return 0;
}

int getLinkofvalue(hashTable *table, int value)             //hash value에 맞는 link of value 반환
{
    for(int i=0;i<101;i++)
    {
        if(table[i].gethvalue()==value)
        {
            return table[i].getlinkvalue();
        }
    }
    return 0;
}

void printline(NodeArray *array, hashTable *table, int i)       // 입력된 원래 문장으로 다시 만드는 함수
{
    while(true)
    {
        if(array[i].getLNode()==0 && array[i].getRNode()==0)
        {
            break;
        }
        if(array[i].getLNode()<0)
        {
            if(array[i].getRNode()==0)
            {
                cout << getHashsymbol(table,array[i].getLNode());
            }
            else if(array[i].getRNode()<0)
            {
                cout << getHashsymbol(table,array[i].getLNode()) << " ";
            }
            else if(array[array[i].getRNode()].getLNode()==0 && array[array[i].getRNode()].getRNode()==0)
            {
                cout << getHashsymbol(table,array[i].getLNode());             
            }
            else 
            {
                cout << getHashsymbol(table,array[i].getLNode()) << " ";
            }        
        }
        else if(array[i].getLNode()>0)
        {
            cout << "(";
            printline(array,table,array[i].getLNode());
            cout << ")";
        }
        if(array[i].getRNode()!=0)
        {
            if(array[i].getRNode()>0)
            {
                printline(array,table,array[i].getRNode());
            }
            else
            {
                cout << getHashsymbol(table, array[i].getRNode());
            }
                                  
        }
        break;
    }
}

void printstack()
{
    sNode * temp = stackTop;
    while(temp!= nullptr)
    {
        cout << "key : "<< temp->key << " ,value : " <<temp->value <<endl;
        temp = temp->next;
    }
}

pair <int,int> Eval(hashTable *table, NodeArray *array, int root)            // Evaluation 함수 뒤의 int는 1이면 true 0이면 list? 2면 value
{
    if(root == -1)
    {
        return {-1,0};
    }
    if(root < 0 && getLinkofvalue(table,root)!=0)
    {
        if(getLinkofvalue(table,root)== -1)
        {
            return {-1,0};
        }
        return {getLinkofvalue(table,root),0};
    }
    else if(getLinkofvalue(table,root)!=0)
    {
        return Eval(table,array,getLinkofvalue(table,root));
    }
    if(root < 0)
    {
        return {root,2};
    }
    string token_index = getHashsymbol(table,array[root].getLNode());
    int value;
    string s;
    if(token_index=="+")
    {
        value = stoi(getHashsymbol(table,Eval(table,array,array[array[root].getRNode()].getLNode()).first)) + stoi(getHashsymbol(table,Eval(table,array,array[array[array[root].getRNode()].getRNode()].getLNode()).first));
        s = to_string(value);
        pushTable2(table,s);
        return {getHashvalue(table,s),2};
    }
    else if(token_index == "-")
    {
        value = stoi(getHashsymbol(table,Eval(table,array,array[array[root].getRNode()].getLNode()).first)) - stoi(getHashsymbol(table,(Eval(table,array,array[array[array[root].getRNode()].getRNode()].getLNode())).first));
        s = to_string(value);
        pushTable2(table,s);
        return {getHashvalue(table,s),2};
    }
    else if(token_index == "*")
    {
        value = stoi(getHashsymbol(table,Eval(table,array,array[array[root].getRNode()].getLNode()).first)) * stoi(getHashsymbol(table,(Eval(table,array,array[array[array[root].getRNode()].getRNode()].getLNode())).first));
        s = to_string(value);
        pushTable2(table,s);
        return {getHashvalue(table,s),2};
    }
    else if(token_index == "/")
    {
        value = stoi(getHashsymbol(table,Eval(table,array,array[array[root].getRNode()].getLNode()).first)) / stoi(getHashsymbol(table,(Eval(table,array,array[array[array[root].getRNode()].getRNode()].getLNode())).first));
        s = to_string(value);
        pushTable2(table,s);
        return {getHashvalue(table,s),2};
    }
    else if(token_index == "%")
    {
        value = stoi(getHashsymbol(table,Eval(table,array,array[array[root].getRNode()].getLNode()).first)) % stoi(getHashsymbol(table,(Eval(table,array,array[array[array[root].getRNode()].getRNode()].getLNode())).first));
        s = to_string(value);
        pushTable2(table,s);
        return {getHashvalue(table,s),2};
    }
    else if(token_index == "number?")
    {
        if(IsNumber(getHashsymbol(table,Eval(table,array,array[array[root].getRNode()].getLNode()).first)) && Eval(table,array,array[array[root].getRNode()].getLNode()).second==2)
        {
            s = "#t";
            pushTable2(table,s);
            return {getHashvalue(table,s),-1};
        }
        s = "#f";
        pushTable2(table,s);
        return {getHashvalue(table,s),1};
    }
    else if(token_index == "symbol?")
    {
        if(IsNumber(getHashsymbol(table,Eval(table,array,array[array[root].getRNode()].getLNode()).first)))
        {
            s = "#f";
            pushTable2(table,s);
            return {getHashvalue(table,s),1};
        }
        s = "#t";
        pushTable2(table,s);
        return {getHashvalue(table,s),-1};
    }
    else if(token_index == "null?")
    {
        if(array[root].getRNode()==0 || Eval(table,array,Eval(table,array,array[root].getRNode()).first).first == -1)
        {
            s = "#t";

            pushTable2(table,s);
            return {getHashvalue(table,s),-1};
        }
        s = "#f";

        pushTable2(table,s);
        return {getHashvalue(table,s),1};

    }
    else if(token_index == "car")
    {

        int result_first;
        int result_second;
        if(Eval(table,array,array[array[root].getRNode()].getLNode()).first<0)
        {
            result_first = Eval(table,array,array[array[root].getRNode()].getLNode()).first;    
        }
        else
        {
            result_first = array[Eval(table,array,array[array[root].getRNode()].getLNode()).first].getLNode();
        }
        if(array[Eval(table,array,array[array[root].getRNode()].getLNode()).first].getLNode()<0)   // car이 List인지 하나의 항목인지 구분
        {
            result_second = 1;
        }
        else
        {
            result_second = 0;
        }
        return {result_first,result_second};
        
    }
    else if(token_index == "cdr")
    {
        if(Eval(table,array,array[array[root].getRNode()].getLNode()).first <0)
        {
            int link_val = getLinkofvalue(table,(array[array[root].getRNode()].getLNode()));

            if(link_val <0)
            {

                return {-1,0};
            }

            return {array[link_val].getRNode(),0};
        }
        else 
        {
            cout << array[Eval(table,array,array[array[root].getRNode()].getLNode()).first].getRNode() <<endl;
            return {array[Eval(table,array,array[array[root].getRNode()].getLNode()).first].getRNode(),0};
        }
    }
    else if(token_index == "cons")
    {
        int new_memory = nodearray_size;
        nodearray_size++;

        array[new_memory].setLNode(Eval(table,array,array[array[root].getRNode()].getLNode()).first);

        array[new_memory].setRNode(Eval(table,array,array[array[array[root].getRNode()].getRNode()].getLNode()).first);
        return {new_memory,0};
    }
    else if(token_index == "cond")
    {
        while(array[array[root].getRNode()].getRNode()!=0)
        {
            root = array[root].getRNode();

            if(Eval(table,array,array[array[root].getLNode()].getLNode()).second==-1)
            {
                return Eval(table,array,array[array[root].getLNode()].getRNode());
            }
        }
        if(getHashsymbol(table,array[array[array[root].getRNode()].getLNode()].getLNode())!="else")
        {
            cout << "Error";
            return {root,-2};
        }
        return Eval(table,array,array[array[array[array[root].getRNode()].getLNode()].getRNode()].getLNode());
    }
    else if(token_index == "quote")
    {
        int index;
        if(array[array[root].getRNode()].getLNode()<0)
        {
            index = 2;
        }
        else
        {
            index = 0;
        }
        return {array[array[root].getRNode()].getLNode(),index};
    }
    else if(token_index == "define")
    {
        if(getHashsymbol(table,array[array[array[array[root].getRNode()].getRNode()].getLNode()].getLNode())=="lambda")
        {
            pushTablelink(table,array[array[root].getRNode()].getLNode(),array[array[array[root].getRNode()].getRNode()].getLNode());
            return {array[array[array[root].getRNode()].getRNode()].getLNode(),3};
        }
        else
        {
            pushTablelink(table,array[array[root].getRNode()].getLNode(),array[array[root].getRNode()].getRNode());
            return {array[array[root].getRNode()].getRNode(),3};
        }
    }
    else if(isinit(table, token_index, 101)==1 && IsNumber(token_index)==false && array[getLinkofvalue(table,array[root].getLNode())].getLNode()==getHashvalue(table,"lambda"))         // user define function
    {
        int count = 0;
        //push current values to stack
        int temp;
        int root_temp;
        pair <int,int> result;
        int hvalue;
        string symbol;

        value = getLinkofvalue(table,array[root].getLNode());       // lambda로 이동
        temp = array[array[value].getRNode()].getLNode();

        if(temp < 0)
        {
            return {temp,2};
        }
        if(array[temp].getLNode()!=-1)
        {
            pushStack(getHashsymbol(table,array[temp].getLNode()),getLinkofvalue(table,array[temp].getLNode()));
            count ++;
        }
        

        if(Eval(table,array,array[root].getRNode()).first == 0)
        {
            pushTablelink(table,array[temp].getLNode(),-1);
        }
        else 
        {
            pushTablelink(table,array[temp].getLNode(),Eval(table,array,array[root].getRNode()).first);
        }

        root_temp = array[root].getRNode();

        while(array[temp].getRNode()!=0 && getLinkofvalue(table,array[temp].getLNode())!=-1)
        {
            temp = array[temp].getRNode();
            pushStack(getHashsymbol(table,array[temp].getLNode()),getLinkofvalue(table,array[temp].getLNode()));
            count++;
            if(Eval(table,array,array[root_temp].getRNode()).first!=0 && Eval(table,array,array[root_temp].getRNode()).first >0)
            {
                pushTablelink(table,array[temp].getLNode(),Eval(table,array,array[root_temp].getRNode()).first);
            }
            root_temp = array[root_temp].getRNode();                                    
        }

        result = Eval(table,array,array[array[array[value].getRNode()].getRNode()].getLNode());
        while(count!=0)
        {
            symbol = stackTop->key;
            hvalue = stackTop->value;
            pushTablelink(table,symbol,hvalue);
            popStack();
            count--;
        }
        return result;


    }
    else if(array[root].getLNode()<0 && getLinkofvalue(table,array[root].getLNode())!=0)
    {
        if(getLinkofvalue(table,array[root].getLNode())==-1)
        {
            return {-1,0};
        }
        return {array[getLinkofvalue(table,array[root].getLNode())].getLNode(),2};
    }
    else if(getLinkofvalue(table,array[root].getLNode())!=0)
    {
        return {array[getLinkofvalue(table,array[root].getLNode())].getLNode(),2};
    }
    else if(array[root].getLNode()<0 && array[root].getRNode()==0 && IsNumber(getHashsymbol(table, array[root].getLNode())))
    {
        return {array[root].getLNode(),2};
    }
    else if(array[root].getLNode()<0 && array[root].getRNode()==0)
    {
        return {root,2};
    }
    else if(array[root].getLNode()>0 && array[root].getRNode()==0)              // () evaluation하는 경우
    {
        return Eval(table,array,array[root].getLNode());        
    }
    
    else if(array[root].getLNode()==0 && array[root].getRNode()==0)             // 비어있는 리스트
    {
        return {-1,0};
    }
    
    else if(array[root].getLNode()>0 && array[root].getRNode()>0)
    {
        return Eval(table,array,array[root].getLNode());
    }
    
    return {-1,0};
}


int main(){                                     // ( 로 시작하는 문장이 아니면 종료. 아니면 무한 반복함.
    hashTable *hTable = new hashTable[101];
    NodeArray *narray = new NodeArray[101];
    int count = 1;
    while(true)
    {
        int i;
        string input;
        getline(cin,input);
        input = Preprocessing(input);
        if(input[0]!='(')
        {
            break;
        }
        if(nodearray_size==0)
        {
            i = read(1,0,input,narray,hTable).first;
        }
        else
        {
            i =read(nodearray_size,0,input,narray,hTable).first;
        }

        nodearray_size = i;
        
        /*
        cout << "Free list's root : " << i << endl;
        cout << "Parse tree's root : 1"<<endl;  
        cout << "Node array :" << endl;
        printNode(narray, nodearray_size);
        cout << "Hash table :" << endl;
        printHash(hTable,101);
        */


        int start, check;
        pair<int,int> result = Eval(hTable, narray,count);
        start = result.first;
        check = result.second;
        count = nodearray_size;
        if(check == 1 || check == -1 || check == 2)
        {
            cout << getHashsymbol(hTable, start)<<endl;
        }
        else if(check == 0)
        {
            cout << "(";
            printline(narray,hTable,start);
            cout << ")" <<endl;
        }
        else if(check == 3)
        {
            continue;
        }
        else
        {
            cout << "Error";
        }

    }
    
    
}