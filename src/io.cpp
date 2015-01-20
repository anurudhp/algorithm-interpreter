#ifdef COMPONENT_PARSER_H
#ifndef COMPONENT_PARSER_IO_H
#define COMPONENT_PARSER_IO_H

namespace IOhandler {
	Token write(Token);
	
	Token read(tokenType);
	Token get();
	Token readLine();
};

#endif /* COMPONENT_PARSER_IO_H */
#endif /* COMPONENT_PARSER_H */
