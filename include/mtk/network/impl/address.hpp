#ifndef MTK_NETWORK_IMPL_ADDRESS_HPP
#define MTK_NETWORK_IMPL_ADDRESS_HPP

#include <mtk/core/types.hpp>

namespace mtk {

extern const int _af_inet;
extern const int _af_inet6;

extern const int _inet_addrstrlen;
extern const int _inet6_addrstrlen;

struct _inet_addr
{
	uint32_t value[4]; // first element for ipv4, all for ipv6, network byte order
};

//af = _af_inet or _af_inet6
//src = character representation of address, must be null terminated
bool _inet_pton(int af, const char* src, _inet_addr& dst) noexcept;

//af = _af_inet or _af_inet6
//dst = character buffer
//size = length of character buffer, must be >= _inet_addrstrlen if af is _af_inet or >= _inet6_addrstrlen if af is _af_inet6
bool _inet_ntop(int af, const _inet_addr& src, char* dst, size_t size) noexcept;


} // namespace mtk

#endif
