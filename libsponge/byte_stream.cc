#include "byte_stream.hh"
// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) : _capacity(capacity),_remain(capacity), _buffer("") {}

size_t ByteStream::write(const string &data) {
    DUMMY_CODE(data);
    size_t r;
    size_t len = data.length();
    if (len >= _remain) {
        r = _remain;
        _buffer.append(data.substr(0, _remain));
        _remain = 0;
        // return r;
    } else {
        r = len;
        _buffer.append(data);
        _remain -= len;
        // return len;
    }
    bytes_w += r;
    return r;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const { return _buffer.substr(0, len); }

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    if (len > buffer_size()) {
        bytes_r += buffer_size();
        _buffer = "";
        _remain = _capacity;
        
    } else {
        bytes_r += len;
        _buffer = _buffer.substr(len);
        _remain += len;
    }
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    string str = peek_output(len);
    pop_output(len);
    // bytes_r += str.length();
    return str;
}

void ByteStream::end_input() {_input_end = true; }

bool ByteStream::input_ended() const { return _input_end; }

size_t ByteStream::buffer_size() const { return _buffer.length(); }

bool ByteStream::buffer_empty() const { return _buffer.empty(); }

bool ByteStream::eof() const { return buffer_empty() && _input_end; }

size_t ByteStream::bytes_written() const { return bytes_w; }

size_t ByteStream::bytes_read() const { return bytes_r; }

size_t ByteStream::remaining_capacity() const { return _remain; }
