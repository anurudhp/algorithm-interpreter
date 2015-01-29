#include "interface.h"

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
	
	return true;
}

int main()
{
	loadData();
	Lexer *lexer = new Lexer("../logs/test.alg");
	
	Parser *parser = new Parser(lexer, "");
	if (!parser->parseSource()) {
		cerr << "\nParsing failed...\n";
		return 0;
	}
	cerr << "Parsing complete.\n";
	
	RPN out = parser->getOutput();
	RPN out2 = out;
	ofstream fout("../logs/rpn.txt"); // log tokens.
	Token tok;
	while (out.pop(tok)) {
		fout << tok.value() << '\t';
		if (tok.value() == ";") fout << endl;
	}
	fout << endl;
	fout.close();
	
	Evaluator *eval = new Evaluator(parser);
	tok = eval->evaluateRPN(out2, VariableScope());
	cout << tok.value() << endl;
	return 0;
}
