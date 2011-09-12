#ifndef __CITE_CMDLINE_HPP__
#define __CITE_CMDLINE_HPP__

#include <yaggo.hpp>

class cite_cmdline {
public:
  bool                           bibtex_flag;
  yaggo::string                  output_arg;
  bool                           output_given;

  enum {
    USAGE_OPT = 1000
  };

  cite_cmdline(int argc, char *argv[]) :
    bibtex_flag(false),
    output_arg("/dev/fd/1"), output_given(false)
  {
    static struct option long_options[] = {
      {"bibtex", 0, 0, 'b'},
      {"output", 1, 0, 'o'},
      {"help", 0, 0, 'h'},
      {"usage", 0, 0, USAGE_OPT},
      {"version", 0, 0, 'V'},
      {0, 0, 0, 0}
    };
    static const char *short_options = "hVbo:";

#define CHECK_ERR(type,val,which) if(!err.empty()) { std::cerr << "Invalid " #type " '" << val << "' for [" which "]: " << err << "\n"; exit(1); }
    while(true) { 
      int index = -1;
      int c = getopt_long(argc, argv, short_options, long_options, &index);
      if(c == -1) break;
      switch(c) {
      case ':': 
        std::cerr << "Missing required argument for "
                  << (index == -1 ? std::string(1, (char)optopt) : std::string(long_options[index].name))
                  << std::endl;
        exit(1);
      case 'h':
        std::cout << usage() << "\n\n" << help() << std::endl;
        exit(0);
      case USAGE_OPT:
        std::cout << usage() << "\nUse --help for more information." << std::endl;
        exit(0);
      case 'V':
        print_version();
        exit(0);
      case '?':
        std::cerr << "Use --usage or --help for some help\n";
        exit(1);
      case 'b':
        bibtex_flag = true;
        break;
      case 'o':
        output_given = true;
        output_arg.assign(optarg);
        break;
      }
    }
    if(argc - optind != 0)
      error("Requires exactly 0 argument.");
  }
#define cite_cmdline_USAGE "Usage: jellyfish cite [options]"
  const char * usage() const { return cite_cmdline_USAGE; }
  void error(const char *msg) { 
    std::cerr << "Error: " << msg << "\n" << usage()
              << "\nUse --help for more information"
              << std::endl;
    exit(1);
  }
#define cite_cmdline_HELP "How to cite Jellyfish's paper\n\nCitation of paper\n\n" \
  "Options (default value in (), *required):\n" \
  " -b, --bibtex                             Bibtex format (false)\n" \
  " -o, --output=string                      Output file (/dev/fd/1)\n" \
  "     --usage                              Usage\n" \
  " -h, --help                               This message\n" \
  " -V, --version                            Version"

  const char * help() const { return cite_cmdline_HELP; }
#define cite_cmdline_HIDDEN "Hidden options:"

  const char * hidden() const { return cite_cmdline_HIDDEN; }
  void print_version(std::ostream &os = std::cout) const {
#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION "0.0.0"
#endif
    os << PACKAGE_VERSION << "\n";
  }
  void dump(std::ostream &os = std::cout) {
    os << "bibtex_flag:" << bibtex_flag << "\n";
    os << "output_given:" << output_given << " output_arg:" << output_arg << "\n";
  }
private:
};

#endif // __CITE_CMDLINE_HPP__"
