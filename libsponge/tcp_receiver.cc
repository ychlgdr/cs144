#include "tcp_receiver.hh"

#include "wrapping_integers.hh"

#include <random>
// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

// #define _LISTEN 1
// #define _SYN_RECV 2
// #define _FIN_RECV 3
// #define _ERROR -1
// _isn{0};
using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    DUMMY_CODE(seg);

    const TCPHeader &header = seg.header();
    const Buffer &payload = seg.payload();
    // if(!header.syn && _state == _SYN_RECV) return;

    if (_state == _LISTEN) {
        if (!header.syn)
            return;
        // _occupy++;
        _isn = header.seqno;
        _state = _SYN_RECV;
    }
    if (_state == _SYN_RECV || _state == _FIN_RECV) {
        const string &str = payload.copy();
        uint64_t index = unwrap(header.seqno, _isn, _checkpoint);
        _checkpoint = index;

        _reassembler.push_substring(str, index - _occupy, header.fin);

        if (header.fin) {
            // _occupy++;
            _end_index = index + str.length();  // 1 + 15 = 16
            _state = _FIN_RECV;
        }
        uint64_t now_index = unwrap(ackno().value(), _isn, _checkpoint);
        if (_state == _FIN_RECV && now_index == _end_index)
            _occupy++;
        if (header.syn) {
            _occupy++;
        }
        // return;
    }
    // } else if (_state == _FIN_RECV) {
    //     // const string &str = payload.copy();
    //     // uint64_t index = unwrap(header.seqno, _isn, _checkpoint);
    //     // _checkpoint = index;

    //     // _reassembler.push_substring(str, index - _occupy, header.fin);
    //     // uint64_t now_index = unwrap(ackno().value(), _isn, _checkpoint);
    //     // if (now_index == _end_index)
    //     //     _occupy++;
    // }
}

optional<WrappingInt32> TCPReceiver::ackno() const {
    // if state is't SYN_RECV,return null
    if (_state == _LISTEN) {
        return nullopt;
    }
    // return ack no wrapped by wrapper
    return wrap(_reassembler.absolute_seq(), _isn) + _occupy;
}

size_t TCPReceiver::window_size() const { return _capacity - _reassembler.stream_out().buffer_size(); }
