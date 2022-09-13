#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity)
    : _output(capacity), _capacity(capacity), _unassembled_str(), _unassembled_interval() {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    size_t len = data.length();
    size_t next = index + len;
    // const string &s = data;
    size_t begin = index;
    size_t end = next - 1;
    // if (index < _next_assembled) {
    //     const string &a = s.substr(_next_assembled - index);
    //     begin = _next_assembled;
    // }
    // size_t begin = index;
    // size_t end = next - 1;

    // check if out of eof range or multily eof
    if (_eof && (next > _eof_index || eof))
        return;

    // check if out of capacity
    size_t m = max_unassembled();
    if (next > m) {
        // const string &s = data.substr(0, m - index);
        // push_substring(s, index, eof);
        return;
    }

    // check if intersect
    // if (index < _next_assembled)
    //     return;
    // for (auto &p : _unassembled_interval) {
    //     if (max(p.first, begin) < min(p.second, end))
    //         return;
    // }

    if (eof) {
        _eof = true;
        _eof_index = next;
    }

    auto cmp = [](pair<size_t, size_t> a, pair<size_t, size_t> b) -> bool {
        if (a.first < b.first)
            return true;
        if (a.first == b.first)
            return a.second <= b.second;
    };

    for (int i = 0; i < _unassembled_interval.size(); i++) {
        if (i + 1 == _unassembled_interval.size()) {
            _unassembled_interval.emplace_back(make_pair(begin, end));
            break;
        }
        auto b = _unassembled_interval[i];
        auto n = _unassembled_interval[i + 1];
        //if pair smaller than before larger than next,then insert it
        if (cmp(b, {begin, end}) && cmp({begin, end}, n)) {
            _unassembled_interval.insert(_unassembled_interval.begin() + i + 1, {begin, end});
            break;
        }
    }
    if (_unassembled_interval.empty()) {
        _unassembled_interval.emplace_back(make_pair(begin, end));
    }
    // _unassembled_interval.emplace(begin, end);
    _unassembled += len;
    _unassembled_str.insert({begin, data});

    for(auto & p : _unassembled_interval){
        if(p.first <= _next_assembled){
            const string &str = _unassembled_str.at(p.first);
            _unassembled_interval.erase(_next_assembled);
            
        }
    }

    // while (_unassembled_str.find(_next_assembled) != _unassembled_str.end()) {
    //     const string &str = _unassembled_str.at(_next_assembled);

    //     _unassembled_str.erase(_next_assembled);
    //     _unassembled_interval.erase(_next_assembled);
    //     _unassembled -= str.length();
    //     _next_assembled += str.length();
    //     // size_t e = _next_assembled + str.length() - 1;

    //     _output.write(str);

    //     if (_eof && _next_assembled == _eof_index)
    //         _output.end_input();
    // }
}

size_t StreamReassembler::unassembled_bytes() const { return _unassembled; }

bool StreamReassembler::empty() const { return _output.buffer_empty() && _unassembled == 0; }
