#ifndef __DUMP_ARGS_HPP__
#define __DUMP_ARGS_HPP__

#include <yaggo.hpp>

class dump_args {
public:
  bool                           column_flag;
  bool                           tab_flag;
  uint64_t                       lower_count_arg;
  bool                           lower_count_given;
  uint64_t                       upper_count_arg;
  bool                           upper_count_given;
  yaggo::string                  output_arg;
  bool                           output_given;
  yaggo::string                  db_arg;

  enum {
    USAGE_OPT = 1000
  };

  dump_args(int argc, char *argv[]) :
    column_flag(false),
    tab_flag(false),
    lower_count_arg(0), lower_count_given(false),
    upper_count_arg(0), upper_count_given(false),
    output_arg(""), output_given(false)
  {
    static struct option long_options[] = {
      {"column", 0, 0, 'c'},
      {"tab", 0, 0, 't'},
      {"lower-count", 1, 0, 'L'},
      {"upper-count", 1, 0, 'U'},
      {"output", 1, 0, 'o'},
      {"help", 0, 0, 'h'},
      {"usage", 0, 0, USAGE_OPT},
      {"version", 0, 0, 'V'},
      {0, 0, 0, 0}
    };
    static const char *short_options = "hVctL:U:o:";

    std::string err;
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
      case 'c':
        column_flag = true;
        break;
      case 't':
        tab_flag = true;
        break;
      case 'L':
        lower_count_given = true;
        lower_count_arg = yaggo::conv_uint<uint64_t>((const char *)optarg, err, false);
        CHECK_ERR(uint64_t, optarg, "-L, --lower-count=uint64")
        break;
      case 'U':
        upper_count_given = true;
        upper_count_arg = yaggo::conv_uint<uint64_t>((const char *)optarg, err, false);
        CHECK_ERR(uint64_t, optarg, "-U, --upper-count=uint64")
        break;
      case 'o':
        output_given = true;
        output_arg.assign(optarg);
        break;
      }
    }
    if(argc - optind != 1)
      error("Requires exactly 1 argument.");
    db_arg = yaggo::string(argv[optind]);
    ++optind;
  }
#define dump_args_USAGE "Usage: jellyfish stats [options] db:path"
  const char * usage() const { return dump_args_USAGE; }
  void error(const char *msg) { 
    std::cerr << "Error: " << msg << "\n" << usage()
              << "\nUse --help for more information"
              << std::endl;
    exit(1);
  }
#define dump_args_HELP "Dump k-mer counts\n\nBy default, dump in a fasta format where the header is the count and the sequence is the sequence of the k-mer. The column format is a 2 column output: k-mer count.\n\n" \
  "Options (default value in (), *required):\n" \
  " -c, --column                             Column format (false)\n" \
  " -t, --tab                                Tab separator (false)\n" \
  " -L, --lower-count=uint64                 Don't output k-mer with count < lower-count\n" \
  " -U, --upper-count=uint64                 Don't output k-mer with count > upper-count\n" \
  " -o, --output=string                      Output file\n" \
  "     --usage                              Usage\n" \
  " -h, --help                               This message\n" \
  " -V, --version                            Version"

  const char * help() const { return dump_args_HELP; }
#define dump_args_HIDDEN "Hidden options:"

  const char * hidden() const { return dump_args_HIDDEN; }
  void print_version(std::ostream &os = std::cout) const {
#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION "0.0.0"
#endif
    os << PACKAGE_VERSION << "\n";
  }
  void dump(std::ostream &os = std::cout) {
    os << "column_flag:" << column_flag << "\n";
    os << "tab_flag:" << tab_flag << "\n";
    os << "lower_count_given:" << lower_count_given << " lower_count_arg:" << lower_count_arg << "\n";
    os << "upper_count_given:" << upper_count_given << " upper_count_arg:" << upper_count_arg << "\n";
    os << "output_given:" << output_given << " output_arg:" << output_arg << "\n";
    os << "db_arg:" << db_arg << "\n";
  }
private:
};

#endif // __DUMP_ARGS_HPP__"
