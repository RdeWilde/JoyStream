#ifndef CHAR_ARRAY_BUFFER
#define CHAR_ARRAY_BUFFER

#include <streambuf>
#include <vector>

class char_array_buffer : public std::streambuf
{
    public:
        char_array_buffer(char *begin, char *end);
        char_array_buffer(std::vector<char> &);

    private:
        // seek operations only for get pointer
        pos_type seekoff(off_type __off, std::ios_base::seekdir __way, std::ios_base::openmode __which);
        pos_type seekpos(pos_type __sp, std::ios_base::openmode __which);

        // seek get pointer to offset position, returns new aboslute position
        pos_type gseekoff(off_type __off, std::ios_base::seekdir __way);

        // seek put pointer to offset position, returns new absolute position
        pos_type pseekoff(off_type __off, std::ios_base::seekdir __way);

        // do not allow copy ctor and assignment
		char_array_buffer(const char_array_buffer &);
		char_array_buffer &operator= (const char_array_buffer &);
};

#endif

