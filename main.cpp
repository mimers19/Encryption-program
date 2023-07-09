/*
Dodatkowe funkcjonalności:

Pobieranie konfiguracji programu z zewnętrznego pliku
Uruchamianie z konsoli
Automatyczne deszyfrowanie
Własny, prosty szyfr podstawieniowy
Szyfr Vigenere'a
*/

/*
Configuration file:
number of lines to show\n
default cypher\n
default encryption/decode\n
custom code [A-Z]

Example:
5
1
0
QWERTYUIOPLKJHGFDSAZXCVBNM
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

struct configuration
{
	int k = 0;				//number of lines to show
	int def = 0;			//0 - Caesar, 1 - Vigenere, 2 - Custom
	bool en_de = 0;
	char cust[26];
};

void conf(configuration* cfg);
void menu(vector <char>* text, int argc, char** argv, configuration cfg);
int validation();
int validation_c(string input);
bool mode();
void caesar(vector <char>* text, int key, bool mode);
bool load_file(vector <char>* text, int argc, char** argv);
void expotr(vector <char>* text, int argc, char** argv, bool wfile);
void view(vector <char>* text, configuration cfg, bool ad, int argc, char** argv);
bool write_line(vector <char>* text, configuration cfg, int w_line, bool file);
int count_lines(vector <char>* text);
void console(vector <char>* text, configuration cfg, int argc, char** argv);
void aut_dec(vector <char>* text, configuration cfg, int argc, char** argv);
char count_char(vector <char>* text, char mct[]);
void custom(vector <char>* text, configuration cfg, bool mode);
string validation_str();
void vigenere(vector <char>* text, bool mode, string key);
string validation_strc(string input);

int main(int argc, char** argv)
{
	configuration cfg;
	vector <char> text[2];
	conf(&cfg);
	

	if (argc == 1)
	{
		load_file(text, argc, argv);
		menu(text, argc, argv, cfg);
		view(text, cfg, false, argc, argv);
	}
	else if (argc == 5)
	{
		console(text, cfg, argc, argv);
	}
	else
	{
		cout << "Wrong number of arguments" << endl;
	}
}

void menu(vector <char>* text, int argc, char** argv, configuration cfg)
{
	system("cls");
	cout << "MENU" << endl << "-------------------------------------" << endl;
	cout << "1. Caesar Cipher" << endl << "2. Vigenere's Cipher" << endl << "3. Custom Cipher" << endl << "4. Automaticly decode Caesar Cipher" << endl << "-------------------------------------" << endl << endl << "> ";
	int cipher;
	string key_s;


	do
	{
		cipher = validation();
		switch (cipher)
		{
		case 1:
			int key;
			system("cls");
			cout << "Enter key to cipher (only numerical value)" << endl << "-------------------------------------" << endl << endl << "> ";
			key = validation();
			caesar(text, key, mode());
			break;
		case 2:
			system("cls");
			cout << "Enter key to cipher (only capital letters):" << endl << "-------------------------------------" << endl << endl << "> ";
			key_s = validation_str();
			vigenere(text, mode(), key_s);
			break;
		case 3:
			custom(text, cfg, mode());
			break;
		case 4:
			aut_dec(text, cfg, argc, argv);
			break;
		default:
			cout << "This is not an option! Try again:" << endl;
		}
	} while (!(cipher == 1 || cipher == 4 || cipher == 3 || cipher == 2));
}

int validation()
{
	string input;
	cin >> input;
	for (int i = -99999; i < 999999; i++)
	{
		if (to_string(i) == input)
		{
			return i;
		}
	}
	return 0;
}

int validation_c(string input)
{
	for (int i = -99999; i < 999999; i++)
	{
		if (to_string(i) == input)
		{
			return i;
		}
	}
	return 0;
}

bool mode()																				//True - encrypt, false - decode
{
	system("cls");
	bool mode = false;							//true - encrypt, false - decode
	cout << "Choose mode you want to use (set default to decoding)" << endl << "-------------------------------------" << endl
		<< "1. Encrypt" << endl << "2. Decode" << endl << "-------------------------------------" << endl << endl << "> ";
	do
	{
		int x = validation();
		if (x == 1)
		{
			return true;
		}
		else if (x == 2)
		{
			return false;
		}
		else
		{
			cout << "Wrong input. Try again:" << endl;
		}
	} while (true);

}

void caesar(vector <char>* text, int key, bool mode)
{
	for (int i = 0; i < text[0].size(); i++)
	{
		int no_char = int(text[0][i]);
		if (int(text[0][i]) >= 65 && int(text[0][i]) <= 90)								    //Big letters
		{
			int buff;
			if (mode == true)																//mode -> true - encrypt 
			{
				buff = (no_char - 65 + key) % 26;
			}
			else                                                                            //mode -> false - decode
			{
				buff = (no_char - 65 - key) % 26;
			}
			if (buff >= 0)
			{
				text[1].push_back(char(buff + 65));
			}
			else
			{
				text[1].push_back(char(buff + 65 + 26));
			}
		}
		else if (int(text[0][i]) >= 97 && int(text[0][i]) <= 122)							//Small letters
		{
			int buff;
			if (mode == true)																//mode -> true - encrypt 
			{
				buff = (no_char - 97 + key) % 26;
			}
			else                                                                            //mode -> false - decode 
			{
				buff = (no_char - 97 - key) % 26;
			}
			if (buff >= 0)
			{
				text[1].push_back(char(buff + 97));
			}
			else
			{
				text[1].push_back(char(buff + 97 + 26));
			}
		}
		else                                                                                //Everything else		
		{
			text[1].push_back(text[0][i]);
		}
	}
	//cout << "Encrypted" << endl;
}

bool load_file(vector <char>* text, int argc, char** argv)
{
	system("cls");
	//cout << "Input file loaded" << endl;
	do
	{
		ifstream input;
		string name;
		if (argc == 5)
		{
			name = argv[3];
		}
		else
		{
			cout << "Insert name of the input file" << endl << "-------------------------------------" << endl << endl << "> ";
			cin >> name;
		}
		input.open(name, ios::in);
		char character;
		if (input.good())
		{
			while (input.get(character))
			{
				text[0].push_back(character);
			}
			input.close();
			return true;
		}
		cout << "Something went wrong. Try again..." << endl;
	} while (true);
}

void expotr(vector <char>* text, int argc, char** argv, bool wfile)
{
	system("cls");
	ofstream out;
	string name;
	if (argc == 5)
	{
		name = argv[4];
	}
	else
	{
		cout << "Insert name of the output file:" << endl << "-------------------------------------" << endl << endl << "> ";
		cin >> name;
	}
	out.open(name, ios::out | ios::trunc);
	for (int i = 0; i < text[wfile].size(); i++)
	{
		out << text[wfile][i];
	}
	//cout << "Solutione exported" << endl;
}

void conf(configuration* cfg)
{
	ifstream cfg_file;
	string buff;
	cfg_file.open("config.cfg", ios::in);
	if (cfg_file.good())
	{
		getline(cfg_file, buff);
		cfg->k = atoi(buff.c_str());
		getline(cfg_file, buff);
		cfg->def = atoi(buff.c_str());
		getline(cfg_file, buff);
		cfg->en_de = atoi(buff.c_str());
		getline(cfg_file, buff);
		for (int i = 0; i < buff.length(); i++)
		{
			cfg->cust[i] = buff[i];
		}
		cfg_file.close();
		//cout << "Configuration file loaded!" << endl;
	}
	else
	{
		cout << "Something went wrong - check configuration file" << endl;
	}
}

void view(vector <char>* text, configuration cfg, bool ad, int argc, char** argv)
{
	int page = 0;
	char option;
	int lines = count_lines(text);
	bool file = true;
	do
	{
		system("cls");
		for (int i = 0 + page * cfg.k; i < ((page + 1) * cfg.k) && i <= lines; i++)
		{
			cout << i << '.' << setw(6);
			write_line(text, cfg, i, file);
		}
		cout << endl << endl << "---------------------------------------------------------------------------------------------------------" << endl << "Manual: " << endl
			<< "n - next page      p - previous page      c - change      q - quit";
			cout << "      a - accept";
		cout << endl << "---------------------------------------------------------------------------------------------------------" << endl << endl << "> ";
		cin >> option;
		switch (option)
		{
		case 'n':
			if (page * cfg.k <= lines)
			{
				page++;
			}
			break;
		case 'p':
			if (page * cfg.k >= 0)
			{
				page--;
			}
			break;
		case 'c':
			file = !file;
			break;
		case 'q':
			exit(0);
			break;
		case 'a':
			expotr(text, argc, argv, file);
			break;
		default:
			cout << "Wrong input. Try again..." << endl;
			system("PAUSE");
		}

	} while (true);
}

bool write_line(vector <char>* text, configuration cfg, int w_line, bool file)
{
	int counter = 0;
	for (int i = 0; i < text[file].size(); i++)
	{
		if (counter == w_line)
		{
			cout << text[file][i];
		}

		if (text[file][i] == '\n')
		{
			counter++;
		}

		if (counter > w_line)
		{
			return true;
		}
	}
	return false;
}

int count_lines(vector <char>* text)
{
	int counter = 0;
	for (int i = 0; i < text[1].size(); i++)
	{
		if (text[1][i] == '\n')
		{
			counter++;
		}
	}
	return counter;
}

void console(vector <char>* text, configuration cfg, int argc, char** argv)
{
	load_file(text, argc, argv);
	bool mode;

	switch (argv[1][1])
	{
	case 's':
		mode = true;
		break;
	case 'd':
		mode = false;
		break;
	default:
		mode = cfg.en_de;
		string ans;
		if (mode == false)
		{
			ans = "decoding";
		}
		else
		{
			ans = "encryption";
		}
		cout << "Wrong enctyption/decode option. Default set to " << ans << endl;
	}

	switch (cfg.def)
	{
	case 0:
		caesar(text, validation_c(argv[2]), mode);
		break;
	case 1:
		vigenere(text, mode, validation_strc(argv[2]));
		break;
	case 2:
		custom(text, cfg, mode);
		break;
	default:
		cout << "Wrong command values" << endl;
	}

	expotr(text, argc, argv, 1);
}

void aut_dec(vector <char>* text, configuration cfg, int argc, char** argv)
{
	char mct[52];
	char mc = count_char(text, mct);
	cout << mc;
	int pol = int(mc) - int('a');
	int eng = int(mc) - int('e');
	vector <char> trial[2];


	for (int i = 0; i < text[0].size(); i++)
	{
		int no_char = int(text[0][i]);
		if (no_char >= 65 && no_char <= 90)													 //Big letters
		{
			int buff[2];

			buff[0] = (no_char - 65 - pol) % 26;
			buff[1] = (no_char - 65 - eng) % 26;
			for (int i = 0; i < 2; i++)
			{
				if (buff[i] >= 0)
				{
					trial[i].push_back(char(buff[i] + 65));									//Trial 0 - pl        Trial 1 - eng
				}
				else
				{
					trial[i].push_back(char(buff[i] + 65 + 26));
				}
			}
		}
		else if (no_char >= 97 && no_char <= 122)											//Small letters
		{
			int buff[2];

			buff[0] = (no_char - 97 - pol) % 26;
			buff[1] = (no_char - 97 - eng) % 26;
			for (int i = 0; i < 2; i++)
			{
				if (buff[i] >= 0)
				{
					trial[i].push_back(char(buff[i] + 97));									//Trial 0 - pl        Trial 1 - eng
				}
				else
				{
					trial[i].push_back(char(buff[i] + 97 + 26));
				}
			}
		}
		else                                                                                //Everything else		
		{
			trial[0].push_back(text[0][i]);
			trial[1].push_back(text[0][i]);
		}
	}

	view(trial, cfg, true, argc, argv);

}

char count_char(vector <char>* text, char mct[])
{
	for (int i = 0; i < text[0].size(); i++)
	{
		int no_char = int(text[0][i]);
		if (no_char >= 65 && no_char <= 90)
		{
			mct[no_char - 65]++;
		}
		else if (no_char >= 97 && no_char <= 122)
		{
			mct[no_char - 97]++;
		}
	}
	int ind = 0;
	for (int i = 0; i < 52; i++)
	{
		if (mct[i] > mct[ind])
		{
			ind = i;
		}
	}
	if (ind >= 25)
	{
		return char(ind + 65);
	}
	else
	{
		return char(ind + 97);
	}

}

void custom(vector <char>* text, configuration cfg, bool mode)
{
	if (mode)
	{
		for (int i = 0; i < text[0].size(); i++)
		{
			int no_char = int(text[0][i]);
			if (no_char >= 65 && no_char <= 90)													//Big letters
			{
				text[1].push_back(cfg.cust[no_char - 65]);
			}
			else if (no_char >= 97 && no_char <= 122)											//Small letters
			{
				text[1].push_back(char(int(cfg.cust[no_char - 97]) + 32));
			}
			else
			{
				text[1].push_back(text[0][i]);
			}
		}
	}
	else
	{
		bool checked;
		for (int i = 0; i < text[0].size(); i++)
		{
			checked = false;
			for (int j = 0; j < 26; j++)
			{
				if (text[0][i] == cfg.cust[j])
				{
					text[1].push_back(char(j + 65));
					checked = true;
					break;
				}
				else if (text[0][i] == char(int(cfg.cust[j]) + 32))
				{
					text[1].push_back(char(j + 65 + 32));
					checked = true;
					break;
				}
			}
			if (checked == false)
			{
				text[1].push_back(text[0][i]);
			}

		}
	}

}

string validation_str()
{
	string input;
	cin >> input;
	bool in = true;
	do
	{
		in = true;
		for (int i = 0; i < input.length(); i++)
		{
			if (int(input[i]) < 65 || int(input[i]) > 90)
			{
				cout << "Wrong input - try again: " << endl << endl << ">   ";
				cin >> input;
				in = false;
			}
		}
	} while (in == false);
	return input;
}

void vigenere(vector <char>* text, bool mode, string key)
{
	int j = 0;
	for (int i = 0; i < text[0].size(); i++, j++)
	{
		int no_char = int(text[0][i]);
		int no_key = int(key[j % key.length()]) - 65;
		if (int(text[0][i]) >= 65 && int(text[0][i]) <= 90)								    //Big letters
		{
			int buff;
			if (mode == true)																//mode -> true - encrypt 
			{
				buff = (no_char - 65 + no_key) % 26;
			}
			else                                                                            //mode -> false - decode
			{
				buff = (no_char - 65 - no_key) % 26;
			}
			if (buff >= 0)
			{
				text[1].push_back(char(buff + 65));
			}
			else
			{
				text[1].push_back(char(buff + 65 + 26));
			}
		}
		else if (int(text[0][i]) >= 97 && int(text[0][i]) <= 122)							//Small letters
		{
			int buff;
			if (mode == true)																//mode -> true - encrypt 
			{
				buff = (no_char - 97 + no_key) % 26;
			}
			else                                                                            //mode -> false - decode 
			{
				buff = (no_char - 97 - no_key) % 26;
			}
			if (buff >= 0)
			{
				text[1].push_back(char(buff + 97));
			}
			else
			{
				text[1].push_back(char(buff + 97 + 26));
			}
		}
		else                                                                                //Everything else		
		{
			text[1].push_back(text[0][i]);
			j--;
		}
	}
	//cout << "Encrypted" << endl;
}

string validation_strc(string input)
{
	for (int i = 0; i < input.length(); i++)
	{
		if (int(input[i]) < 65 || int(input[i]) > 90)
		{
			cout << "Wrong input - default key is active " << endl << endl << ">   ";
			return "key";
		}
	}
	return input;
}