// Macros for debugging:
int debugLevel = 0;
#define DEB          if (debugLevel >= 3) cerr << ">> REACHED <<\n";
#define PRINT(i)     if (debugLevel >= 2) cerr << ">> " << i << endl;
#define PRINT2(i)    if (debugLevel >= 2) cerr << ">>>>> " << i << endl;
#define DEBUG(i)     if (debugLevel >= 3) cerr << ">> " << #i << " = " << i << endl;
#define DEBUG2(i)    if (debugLevel >= 3) cerr << ">>>> " << #i << " = " << i << endl;
#define LOG(i)       if (debugLevel >= 1) cerr << i;
#define LOGN(i)      if (debugLevel >= 1) cerr << i << endl;

#include "interface.h"

int main()
{
	loadData();
	String msg;
	showConsole(true);
	while (1) {
		msg.get(cin, '\n');
		if (msg == "quit") {
			cout << "Thank you for using the interpreter.\n";
			return 0;
		}
		parseCommand(msg);
	}
	return 0;
}

void parseCommand(String message) {
	if (!message) {
		showConsole();
		return;
	}

	Vector<String> parts = strsplit(message, ' ');
	String cmd(parts[0]);
	parts.popfront();

	if (cmd == "run") {
		runProgram(parts);
	}
	else if (cmd == "clear") {
		system("cls");
	}
	else if (cmd == "help") {
		cout << "AlgoFlex Interpreter console.\n"
		     << "commands:\n"
			 << "help        : displays this help\n"
			 << "about       : displays the credits\n"
			 << "run <file>  : compiles and runs `file`\n"
			 << "clear       : clears the screen\n"
			 << "quit        : clears the data, and shuts down the console.\n"
			 << "\nReport bugs at https://github.com/codelegend/algorithm-compiler.git/\n";
	}
	else if (cmd == "about" || cmd == "credits") {
		cout << "Algo Flex Interpreter (Version 1.0.0 (beta))\n"
		     << "Developed by P Sai Anurudh Reddy and V Thejas\n";
	}
	else if (cmd == "debug") {
		String val = strjoin(parts);
		if (val.isInteger()) {
			debugLevel = val.toInteger();
			cout << "Debug level set to " << debugLevel;
		}
	}
	else if (cmd == "hotpatch") {
		loadData();
	}
	else {
		cout << "invalid command. type `help` to see list of commands.";
	}

	showConsole();
}

bool runProgram(Vector<String> args) {
	if (args.size() < 1) {
		cerr << "Invalid usage, expected filename.";
		return false;
	}

	// spawn the lexer, parser and evaluator handles.
	Lexer *lexer = new Lexer(args[0]);
	Parser *parser = new Parser(lexer);
	if (!parser->parseSource()) {
		cerr << "\nParsing failed...\n";
		delete lexer;
		delete parser;
		return false;
	}
	LOGN("Parsing complete.\n");
	if (debugLevel >= 1) logRPN(parser->getOutput());

	Evaluator *evaluator = new Evaluator(parser);
	bool res = evaluator->runProgram();
	if (!res) cerr << "\nEvaluation failed...\n";

	delete lexer;
	delete parser;
	delete evaluator;

	return res;
}
void showConsole(bool firsttime) {
	if (firsttime) {
		cout << "Welcome to the AlgoFlex CLI (Beta release)\n"
		     << "type `help` to see the list of commands\n";
	}
	cout << "\nconsole> ";
}
bool loadData() {
	bool success = true;
	// Load the data required: lexer data and error codes.
	ifstream fin;

	LOG("Loading lexer data...");
	if (!loadLexerData()) {
		LOGN(" unable to load");
		success = false;
	} else {
		LOGN("done.");
	}

	fin.open("data/errorcodes.txt");
	LOG("Loading error codes and descriptions...");
	if (!importErrorCodes(fin)) {
		LOGN(" unable to load");
		success =  false;
	} else {
		LOGN("done.");
	}
	fin.close();

	// some parser settings.
	nullVariableRef.setValue(nullvalToken);

	return success;
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
