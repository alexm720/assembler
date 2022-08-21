#include <unordered_map>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <bits/stdc++.h>
#include <cctype>
#include <cstring>

using namespace std;

string IntToChar(int rem)
{
          int array[7] = {10,11,12,13,14,15,16};
          string Carray = "ABCDEFG";
          string m = "";
          for (int i = 0; i<7; i++)
          {
                  if(rem == array[i])
                  {
                          m += Carray[i]; 
                          return m;
                 }       
          }          
          return to_string(rem);  
}


string BaseTenToN(long long int s, int base)
{
	long long int BaseTen = s;
        int count = 0;
        int i = 0;
        string p = "";
    	string temp;
        while(BaseTen!=0)
        {
		count = BaseTen % base;
                temp = IntToChar(count);
                p += temp;
             	BaseTen = trunc(BaseTen/base);
           	i++;
    	}

	if(p.length() == 3)
	{
		reverse(p.begin(), p.end());
	}
	else if(p.length() == 2)
	{
		p+= "0";
		reverse(p.begin(), p.end());
	}
	else if(p.length() == 1)
	{
		p+= "00";	
		reverse(p.begin(), p.end());
	}
	else if(p.length() == 0)
	{
		p = "000";
	}


          return p;
  }

string ConvertToMachine(string str,unordered_map<string, string> SymTable)
{
	string MachineCode = "";
	stringstream ss(str);
	string temp;
	string check = "";
	int i = 1;
	unordered_map<string, string>::const_iterator look;
	unordered_map<string, string> AllAdd;
	unordered_map<string, string> LessAdd;
	unordered_map<string, string> AddMode;
	unordered_map<string, string> NoAdd;	

	//ADDRESSING MODE 0-9	
	AllAdd["LDA"] = "00";
	AllAdd["LDX"] = "01";
	AllAdd["LDS"] = "02";
	AllAdd["LDF"] = "03";
	AllAdd["PSH"] = "08";
	AllAdd["ADA"] = "10";
	AllAdd["ADX"] = "11";
	AllAdd["ADS"] = "12";
	AllAdd["ADF"] = "13";
	AllAdd["SBA"] = "14";
	AllAdd["SBX"] = "15";
	AllAdd["SBS"] = "16";
	AllAdd["SBF"] = "17";
	AllAdd["MUL"] = "18";
	AllAdd["DIV"] = "19";
	AllAdd["MOD"] = "1A";
	AllAdd["CMA"] = "20";
	AllAdd["CMX"] = "21";
	AllAdd["CMS"] = "22";
	AllAdd["CMF"] = "23";

	//ADDRESSING MODE 2-9
	LessAdd["STA"] = "04";
	LessAdd["STX"] = "05";
	LessAdd["STS"] = "06";
	LessAdd["STF"] = "07";
	LessAdd["POP"] = "09";
	LessAdd["CLR"] = "0A";
	LessAdd["SET"] = "0B";
	LessAdd["JSR"] = "41";
	LessAdd["JEQ"] = "48";
	LessAdd["JLT"] = "4A";
	LessAdd["JGE"] = "4B";

	//ADDRESSING MODE
	AddMode["#"] = "0";
	AddMode["+"] = "4";
	AddMode["*"] = "6";
	AddMode["&"] = "8";
	AddMode["# !"] = "1";
	AddMode["!"] = "3";
	AddMode["+ !"] = "5";
	AddMode["* !"] = "7";
	AddMode["& !"] = "9";

	//NO ADDRESSING MODE
	NoAdd["PSHA"] = "FFF010";
	NoAdd["PSHX"] = "FFF011";
	NoAdd["PSHF"] = "FFF012";
	NoAdd["POPA"] = "FFF013";
	NoAdd["POPX"] = "FFF014";
	NoAdd["POPF"] = "FFF015";
	NoAdd["SHRA"] = "FFF022";
	NoAdd["SHLA"] = "FFF023";
	NoAdd["RTN"] = "FFF040";
	NoAdd["NOP"] = "FFF038";
	NoAdd["HLT"] = "FFFFFF";



	string checkBoth = ""; 

	while(!ss.eof())
	{
		ss >> temp;
		if(temp[0] == '.')
		{	
			ss >> temp;
			if(temp.length() != 6)
			{
				string fill(6-temp.length(),'0');
				MachineCode += fill;
				MachineCode += temp;
			}
			else
			{
				MachineCode += temp;
			}
			return MachineCode;
		}
		look = NoAdd.find(temp);
		if(look != NoAdd.end())
		{
			string checkAdd = temp;
			ss >> temp;
			if(temp.compare(checkAdd) == 0)
			{
				MachineCode = NoAdd[checkAdd];
				return MachineCode;
			}
			else
			{
				return "ASSEMBLER ERROR: instruction using unsupported addressing mode.";
;
			}
		}

		look = AllAdd.find(temp);	
		if(look != AllAdd.end())
		{
			MachineCode += AllAdd[temp];
			ss >> temp;
			if(temp.compare("#") == 0 || temp.compare("!") ==0 || temp.compare("+")==0 ||
			   temp.compare("*") == 0 || temp.compare("&") == 0)
			{
				checkBoth = temp;
				ss >> temp;
				if(temp.compare("!") == 0)
				{
					checkBoth += " ";
					checkBoth += "!";
					ss >> temp;
				}
				look = AddMode.find(checkBoth);
				if(look != AddMode.end())
				{
					MachineCode += AddMode[checkBoth];
				}
			}
			else
			{
				MachineCode += "2";
			}
			string hold = "";
			string one = "00";	
			string two = "0";
			if(temp[0] == '$')
			{
				while(temp[i] != '\0')
				{
					hold += temp[i];
					i++;
				}
				if(hold.length() == 3)
				{
					MachineCode += hold;
					return MachineCode;
				}	
				else if(hold.length()==2)
				{
					MachineCode += two;
					MachineCode += hold;
					return MachineCode;
				}
				else if(hold.length()==1)
				{
					MachineCode += one;
					MachineCode += hold;
					return MachineCode;
				}
			}
			else if(islower(temp[0]))
			{
				look = SymTable.find(temp);		
				if(look != SymTable.end())
				{
					MachineCode += SymTable[temp];
				}
				else
				{
					MachineCode = "ASSEMBLER ERROR: undefined label.";
					return MachineCode;
				}
			}
			else
			{		
				i = stoi(temp);
				hold = BaseTenToN(i,16);
				MachineCode += hold;
			}
							
		}
		
		look = LessAdd.find(temp);	
		if(look != LessAdd.end())
		{
			MachineCode += LessAdd[temp];
			ss >> temp;
			if(temp.compare("#") == 0)
			{
				MachineCode = "ASSEMBLER ERROR: instruction using unsupported addressing mode.";
				return MachineCode;
			}
			if(temp.compare("!") ==0 || temp.compare("+")==0 ||
			   temp.compare("*") == 0 || temp.compare("&") == 0)
			{
				checkBoth = temp;
				ss >> temp;
				if(temp.compare("!") == 0)
				{
					checkBoth += " ";
					checkBoth += "!";
					ss >> temp;
				}
				look = AddMode.find(checkBoth);
				if(look != AddMode.end())
				{
					MachineCode += AddMode[checkBoth];
				}
			}
			else
			{
				MachineCode += "2";
			}
			string hold = "";
			string one = "00";	
			string two = "0";
			if(temp[0] == '$')
			{
				while(temp[i] != '\0')
				{
					hold += temp[i];
					i++;
				}
				if(hold.length() == 3)
				{
					MachineCode += hold;
					return MachineCode;
				}	
				else if(hold.length()==2)
				{
					MachineCode += two;
					MachineCode += hold;
					return MachineCode;
				}
				else if(hold.length()==1)
				{
					MachineCode += one;
					MachineCode += hold;
					return MachineCode;
				}
			}
			else if(islower(temp[0]))
			{
				look = SymTable.find(temp);		
				if(look != SymTable.end())
				{
					MachineCode += SymTable[temp];
				}
				else
				{
					MachineCode = "ASSEMBLER ERROR: undefined label.";
					return MachineCode;
				}
			}
			else
			{		
				i = stoi(temp);
				hold = BaseTenToN(i,16);
				MachineCode += hold;
			}
		}


	}

	return MachineCode;
}	






int main(int argc,char const *argv[])
{

	if(argc!= 3)
	{
		return 0;
	}

	ifstream input_file(argv[1]);

	if(!input_file.is_open())
	{
		return 0;
	}

	unordered_map<string, string> SymbolTable;
	unordered_map<string, string>::const_iterator check;
	string str;
	string add;
	string temp;
	int i = 0;
	while(getline(input_file,str) && i < 4096)
	{
		stringstream ss(str);
		getline(ss,temp,':');
		while(islower(temp[0]) && !isspace(temp[0]))
		{
			check = SymbolTable.find(temp);
			if(check == SymbolTable.end())
			{
				add = BaseTenToN(i,16);
				SymbolTable[temp] = add;
			}
			else
			{
				cerr << "ASSEMBLER ERROR: label defined multiple times." << endl;	
				return 0;
			}
			getline(input_file,str);
			stringstream ss(str);
			getline(ss,temp,':');
		}
		i++;
	}
	string ErrorLabel = "ASSEMBLER ERROR: undefined label.";
	string ErrorOp = "ASSEMBLER ERROR: instruction using unsupported addressing mode.";


	input_file.clear();
	input_file.seekg(0);
	i = 0;
	string test;
	ofstream output_file(argv[2]);


	while(getline(input_file,str) && i < 4096)
	{
		while(islower(str[0]) && !isspace(str[0]))
		{
			getline(input_file,str);
		}
		if(!str.empty())
		{
			test = ConvertToMachine(str,SymbolTable);
			if(!test.empty())
			{
				if(test.compare(ErrorLabel) == 0 || test.compare(ErrorOp) == 0)
				{
					cerr << test << endl;
					return 0;
				}
				output_file << test << endl;
			}
		}
		i++;
	}

	input_file.close();
	output_file.close();
	return 0;


}



