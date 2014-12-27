/***
* Extraction Module
* Takes code from a given file,
* and replaces all non-library includes with the actual source
***/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
const bool success = true;

class Extractor
{
	private:
	string mainfile, output;
	fstream writer;
	vector<string> included;
	
	public:
	Extractor(string mainfile, string output) {
		this->mainfile = mainfile;
		this->output = output;
		writer.open(output.c_str(), ios::out);
		included.resize(0);
	}
	~Extractor() {
		if(writer.is_open()) {
			writer.flush();
			writer.close();
		}
	}
	string getDir(string bsf) {
		bool exdir = 0;
		for(int i = 0; i < bsf.length(); i++) {
			if( bsf[i] == '/' ){
				exdir = 1;
				break;
			}
		}
		if( !exdir )
			return "";
		int lastspos = -1;
		for(int i = bsf.length()-1; i >= 0; i--) {
			if( bsf[i] == '/' ){
				lastspos = i;
				break;
			}
		}
		string ret("");
		for(int i = 0; i <= lastspos; i++) {
			ret += bsf[i];
		}
		return ret;
	}
	string getIncludeName( string line, string basefile ){
		string ign("#ignore");
		if( line.substr(0,9) == "#include\"" ){
			line = line.substr(9);
		} else if( line.substr(0,10) == "#include \"" ){
			line = line.substr(10);
		} else {
			return string("");
		}
		string ret(getDir( basefile ));
		for( int i=0; i<line.length() && line[i]!='"' ; i++){
			ret+=line[i];
		}
		if( ret.length()>0){
			for( int i=0; i<included.size(); i++)
				if( included[i] == ret )
					return ign;
			included.push_back( ret );
		}
		return ret;
	}
	bool extractFile( string fname ){
		fstream fin(fname.c_str(), ios::in);
		if( !fin.is_open() ) return false;
		string st, inc;
		while( !fin.eof() ){
			getline( fin, st );
			// check for include :
			inc = ( st[0] == '#' && st[1] == 'i' ) ? getIncludeName( st, fname ) : "";
			if( inc.length()>0 ){
				if( inc == "#ignore" ){
					cerr << "Ignored : " << st << endl;
				} else if( extractFile( inc ) ){
					cerr << "File " << inc << " loaded!\n";
				} else {
					cerr << "Unable to load file " << inc << "!\n";
				}
			} else {
				writer.write(st.c_str(), st.length());
				writer.put('\n');
			}
		}
		fin.close();
		return success;
	}
	bool process() {
		bool ok = extractFile(mainfile);
		if(ok) {
			cout << "Extraction complete.";
		} else {
			cout << "Extraction failed.";
		}
		return ok;
	}
};

int main()
{
	string a,b;
	cin >> a >> b;
	Extractor e(a, b);
	e.process();
	return 0;
}
