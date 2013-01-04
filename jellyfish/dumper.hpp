/*  This file is part of Jellyfish.

    Jellyfish is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Jellyfish is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Jellyfish.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __JELLYFISH_DUMPER_HPP__
#define __JELLYFISH_DUMPER_HPP__

#include <iostream>
#include <sstream>
#include <fstream>
#include <jellyfish/err.hpp>
#include <jellyfish/time.hpp>

/**
 * A dumper is responsible to dump the hash array to permanent storage
 * and zero out the array.
 **/
namespace jellyfish {
  class dumper_t {
    Time writing_time_;
    int  index_;
    bool one_file_;

  public:
    define_error_class(ErrorWriting);

  protected:
    /// Open the next file with given prefix. If one_file is false,
    /// append _0, _1, etc. to the prefix for actual file name. If
    /// one_file is true, the prefix is the file name. The first time
    /// the file is open in trunc mode, the subsequent times in append
    /// mode.
    void open_next_file(const char *prefix, std::ofstream &out) {
      std::ostringstream name;
      name << prefix;
      std::ios::openmode mode = std::ios::out;
      if(one_file_) {
        mode |= (index_++ ? std::ios::ate : std::ios::trunc);
      } else {
        name << index_++;
        mode |= std::ios::trunc;
      }

      out.open(name.str().c_str());
      if(out.fail())
        eraise(ErrorWriting) << "'" << name.str() << "': "
                             << "Can't open file for writing" << err::no;
    }

  public:
    dumper_t() : writing_time_(::Time::zero), index_(0), one_file_(false) {}

    void dump() {
      Time start;
      _dump();
      Time end;
      writing_time_ += end - start;
    }

    bool one_file() const { return one_file_; }
    void one_file(bool v) { one_file_ = v; }

    virtual void _dump() = 0;
    Time get_writing_time() const { return writing_time_; }
    int nb_files() const { return index_; }
    virtual ~dumper_t() {};
  };
}
#endif // __DUMPER_HPP__
