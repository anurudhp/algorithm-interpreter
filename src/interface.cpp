// Macros for debugging:
int debugLevel = 1;
#define DEB       if (debugLevel > 2) cerr << ">> REACHED <<\n";
#define PRINT(i)  if (debugLevel > 1) cerr << ">> " << i << endl;
#define PRINT2(i)  if (debugLevel > 1) cerr << ">>>>> " << i << endl;
#define DEBUG(i)  if (debugLevel > 2) cerr << ">> " << #i << " = " << i << endl;
#define DEBUG2(i) if (debugLevel > 2) cerr << ">>>> " << #i << " = " << i << endl;
#define LOG(i)    if (debugLevel > 0) cerr << i;
#define LOGN(i)   if (debugLevel > 0) cerr << i << endl;

#include "interface.h"

bool loadData();
void logRPN(RPN);

int main()
{
	cerr << "Debug Level: ";
	cin >> debugLevel; cin.get();
	if (!loadData()) return 0;
	
	Lexer *lexer = new Lexer("../logs/test.alg");
	Parser *parser = new Parser(lexer);
	if (!parser->parseSource()) {
		cerr << "\nParsing failed...\n";
		return 0;
	}
	LOGN("Parsing complete.\n");
	logRPN(parser->getOutput());
	
	cout << "_________________________________________\n\n";
	Evaluator *eval = new Evaluator(parser);
	bool res = eval->runProgram();
	cout << "\n_________________________________________\n";
	if (!res) cerr << "\nEvaluation failed...\n";
	return 0;
}

bool loadData() {
	ifstream fin;
	
	fin.open("data/lexerdata.txt");
	LOG("Loading lexer data...");
	if (!importLexerData(fin)) {
		cerr << " unable to load\n";
		fin.close();
		return false;
	}
	LOGN("done.");
	fin.close();
	
	fin.open("data/errorcodes.txt");
	LOG("Loading error codes and descriptions...");
	if (!importErrorCodes(fin)) {
		cerr << " unable to load\n";
		fin.close();
		return false;
	}
	LOGN("done.");
	fin.close();
	
	nullVariableRef.setValue(Token("null", LITERAL, CONSTANT));
	
	cerr << endl;
	return true;
}

void logRPN(RPN r) {
	ofstream fout("../logs/rpn.txt"); // log tokens.
	Token tok;
	while (r.pop(tok)) {
		fout << tok.value() << '\t';
		if (tok.value() == ";") fout << endl;
	}
	fout << endl;
	fout.close();
	LOGN("RPN logged.");
}
