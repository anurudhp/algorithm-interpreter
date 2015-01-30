// Macros for debugging:
#define DEB       cerr << ">> REACHED <<\n";
#define PRINT(i)  cerr << ">> " << i << endl;
#define DEBUG(i)  cerr << ">> " << #i << " = " << i << endl;

#include "interface.h"

bool loadData();
void logRPN(RPN);

int main()
{
	loadData();

	Lexer *lexer = new Lexer("../logs/test.alg");
	Parser *parser = new Parser(lexer);
	if (!parser->parseSource()) {
		cerr << "\nParsing failed...\n";
		return 0;
	}
	cerr << "Parsing complete.\n";
	logRPN(parser->getOutput());
	
	cout << "_________________________________________\n";
	Evaluator *eval = new Evaluator(parser);
	bool res = eval->runProgram();
	cout << "_________________________________________\n";
	if (!res) cerr << "\nEvaluation failed...\n";
	return 0;
}

bool loadData() {
	ifstream fin;
	
	fin.open("data/lexerdata.txt");
	cerr << "Loading lexer data...";
	if (!importLexerData(fin)) {
		cerr << " unable to load\n";
		fin.close();
		return false;
	}
	cerr << " done.\n";
	fin.close();
	
	fin.open("data/errorcodes.txt");
	cerr << "Loading error codes and descriptions...";
	if (!importErrorCodes(fin)) {
		cerr << " unable to load\n";
		fin.close();
		return false;
	}
	cerr << " done.\n";
	fin.close();
	
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
	cerr << "RPN logged.\n";
}
