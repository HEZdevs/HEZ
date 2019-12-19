#include <fstream>
#include <cstdlib>
#include <cstdarg>

namespace hez { namespace logger {
	std::ofstream fx;
	void start(std::string fileNameWithoutExtension) {
		fx.open(fileNameWithoutExtension + ".rtf");
		fx << "{\\rtf1\\ansi\\deff0{\\colortbl;\\red0\\green0\\blue0;\\red200\\green200\\blue5;\\red255\\green0\\blue0;}Started logging.{\\line}";
		//fx << "{\\fonttbl {\\f0 Courier;}}\n{\\colortbl;\\red0\\green0\\blue0;\\red255\\green255\\blue0;\\red255\\green0\\blue0;}\nStarted logging.\n";
		std::atexit([]{ fx << "\\cf1Ended logging.{\\line}}"; fx.close(); });
	}
	namespace {
		inline const char* timeString() {
			return "[--:--:--]";
		}
		
		const char* prec[3] = { "\\cf1", "\\cf2", "\\cf3" };
		const char* pres[3] = { "", "Warning: ", "ERROR: " };
	}
	void log(int prei, const char* str, ...) {
		va_list args;
    	va_start(args, str);
    	fx << prec[prei] << timeString() << pres[prei];
    	bool bold = false, italic = false, underlined = false, strikethrough = false;
    	while(*str != '\0') 
    	{
    		if(*str == '%') {
    			++str;
    			switch(*str) {
    				case 'i': fx << (va_arg(args, int)); break;
    				case 'f': fx << (float)(va_arg(args, double)); break;
    				case 'd': fx << (va_arg(args, double)); break;
    				case 'c': fx << (char)(va_arg(args, int)); break; 
    				case 'B': fx << ((bool)va_arg(args, int) ? "TRUE" : "FALSE"); break;
    				case 'b': fx << ((bool)va_arg(args, int) ? "true" : "false"); break;
    				case '%': fx << '%'; break; 
    				case 'n': fx << "{\\line}"; break;
				}  
			} else if(*str == '^') {
				++str;
				switch(*str) {
					case '^': fx << '^'; break;
					case 'b': if(bold) fx << "\\b0 "; else fx << "\\b "; bold = !bold; break;
					case 'i': if(italic) fx << "\\i0 "; else fx << "\\i "; italic = !italic; break;
					case 'u': if(underlined) fx << "\\ul0 "; else fx << "\\ul "; underlined = !underlined; break;	
					case 's': if(strikethrough) fx << "\\strike0 "; else fx << "\\strike "; strikethrough = !strikethrough; break;	
				}
			} else fx << (*str);
    		++str;
    	}
    	va_end(args);
		fx << "{\\line}";
	}
} }
