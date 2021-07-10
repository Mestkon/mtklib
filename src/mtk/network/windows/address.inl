#include <mtk/network/impl/address.hpp>

#include <mtk/core/assert.hpp>

#include <ws2tcpip.h>
#include <Wsk.h>

namespace mtk {

const int _af_inet = AF_INET;
const int _af_inet6 = AF_INET6;

const int _inet_addrstrlen = 16;
const int _inet6_addrstrlen = 46;


bool _inet_pton(int af, const char* src, _inet_addr& dst) noexcept
{
	MTK_ASSERT((af == AF_INET) || (af == AF_INET6));
	MTK_ASSERT(src);

	return ::InetPton(af, src, dst.value) == 1;
}

bool _inet_ntop(int af, const _inet_addr& src, char* dst, size_t size) noexcept
{
	MTK_ASSERT((af == AF_INET) || (af == AF_INET6));
	MTK_ASSERT(dst);

	return ::InetNtop(af, src.value, dst, size) != nullptr;
}

} // namespace mtk
