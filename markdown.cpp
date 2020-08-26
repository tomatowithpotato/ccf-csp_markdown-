#include <stdio.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>
#include <utility>
#include <set>
#include <iostream>

using namespace std;

//项目列表和段落各自分开放在不同字符串里
//同一个项目列表的项目在一个字符串里
/*
is_proj|is_para|        情况       |     措施     |
0      |0      |上一个输入为空行    |需创建新项/段 |
0      |1      |上一个输入为段落    |   新项旧段   |
1      |0      |上一个输入为项目    |   新段旧项   |
*/

vector<string> paragraphs;
int w;
int x;
bool is_proj; //判断当前是否正在处理项目
bool is_para;
int row_cnt;

void execute(){
    for(string &s : paragraphs){
        cout << s << endl;
        cout << endl;
    }
}

//超终端，换行
bool huanhang(string &now, string &s, int &i, int end){
    if(x && (x % w == 0)){ //超过终端宽度
        x = 0;
        now.push_back('\n');
        ++row_cnt;
        while(i <= end && s[i] == ' ')++i; //跳过段落中的行首连续空格
        if(is_proj){
            now.push_back(' ');
            now.push_back(' ');
            now.push_back(' ');
            x += 3;
        }
        return true;
    }
    return false;
}

void handle_para(string &now, string &s, int n, int &i){
    int end = n-1;
    while(i < n && s[i] == ' ')++i; //去除首部空格
    while(s[end] == ' ')--end; //去掉末尾空格
    if(now.size() && now.back() != ' ' && !huanhang(now, s, i, end)){
        now.push_back(' '); //换行变空格
        ++x;
    }
    while(i <= end){
        huanhang(now, s, i, end);
        now.push_back(s[i++]);
        ++x;
    }
}

void handle_proj(string &now, string &s, int n, int &i){
    if(i == 0){
        x = 0;
        //非项目首行，故需换行
        if(now.size()){
            ++row_cnt;
            now.push_back('\n');
        }
        i += 2; //跳过'*'和' '
        now.push_back(' ');
        now.push_back('~');
        now.push_back(' ');
        x += 3;
    }
    handle_para(now, s, n, i);
}

int main(){
    ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    is_proj = false;
    is_para = false;
    string s = "\n";
    x = 0;
    row_cnt = 0;

    cin >> w;

    getline(cin, s); //清除缓存

    
    while(getline(cin, s)){
        int i = 0, n = s.size();
        
        while(i < n && s[i] == ' ')++i; //去除首部空格

        //判断空行
        if(i < n){
            //判断是项目还是段落
            if(n > 1 && (s[0] == '*' && s[1] == ' '||
                is_proj && s[0] == ' ' && s[1] == ' '))
            {
                //处理新项目
                if(!is_para && !is_proj || is_para){
                    is_proj = true;
                    is_para = false;
                    x = 0;
                    row_cnt += 2;
                    paragraphs.push_back("");
                }
                handle_proj(paragraphs.back(), s, n, i);
            }
            else {
                //处理新段落
                if(!is_para && !is_proj || is_proj){
                    is_para = true;
                    is_proj = false;
                    x = 0;
                    row_cnt += 2;
                    paragraphs.push_back("");
                    
                }
                handle_para(paragraphs.back(), s, n, i);                        
            }
        }
        else{
            //处理项目列表或段落的结束
            is_proj = is_para =false;
            x = 0;
        }
    }

    execute();
    cout << row_cnt << endl;
    return 0;
}